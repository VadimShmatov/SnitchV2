#pragma once
#include <mpir.h>
#include <gmp-impl.h>
#include <algorithm>

#include "BitLength.h"

using namespace std;

namespace ec_lib
{

	class Power
	{
	protected:

		void Init(const mp_limb_t* limbs, size_t number_of_limbs)
		{
			if (number_of_limbs == 0)
			{
				q = 2;
				d = 1;
				windows_number = 0;
				return;
			}

			number_of_bits = 0;
			while (number_of_limbs > 0)
			{
				unsigned long number_of_bits_in_limb = GetBitLength(limbs[number_of_limbs - 1]);
				if (number_of_bits_in_limb)
				{
					number_of_bits = (number_of_limbs - 1) * 8 * sizeof(mp_limb_t);
					break;
				}
				else
				{
					number_of_limbs--;
				}
			}
			number_of_bits += GetBitLength(limbs[number_of_limbs - 1]);

			q = 2;
			d = 1;
			mp_bitcnt_t tmp = 4;
			while (tmp <= number_of_bits)
			{
				d++;
				tmp <<= 2;
			}

			windows = (window*)malloc(sizeof(window) * (2 * number_of_bits / (q + 1) + 1));
			windows_number = 0;
			unsigned char buf = 0;
			unsigned char buf_size = 0;
			mp_bitcnt_t delta = 0;
			mp_bitcnt_t position;
			for (position = mpn_scan1(limbs, 0); position < number_of_bits; position += delta + 1)
			{
				mp_bitcnt_t next_1 = mpn_scan1(limbs, position);
				delta = next_1 - position;
				if (delta >= q)
				{
					if (buf_size > 0)
					{
						windows[windows_number].pos = number_of_bits + buf_size - 1 - position;
						windows[windows_number].val = buf;
						windows_number++;
					}
					buf = 1;
					buf_size = 1;
				}
				else
				{
					if (buf_size + delta + 1 <= d)
					{
						buf_size += delta + 1;
						buf |= (1 << (buf_size - 1));
					}
					else
					{
						if (buf != 0)
						{
							windows[windows_number].pos = number_of_bits + buf_size - 1 - position;
							windows[windows_number].val = buf;
							windows_number++;
						}
						buf = 1;
						buf_size = 1;
					}
				}
			}
			if (buf != 0)
			{
				windows[windows_number].pos = number_of_bits + buf_size - 1 - position;
				windows[windows_number].val = buf;
				windows_number++;
			}

			if (windows_number > 0)
			{
				windows = (window*)realloc(windows, sizeof(window) * windows_number);
			}
			else
			{
				free(windows);
				windows = nullptr;
			}
		}

	public:
		struct window
		{
			unsigned long long pos;
			unsigned char val;
		};

		window* windows;
		unsigned long long number_of_bits;
		unsigned int windows_number;
		unsigned char q;
		unsigned char d;
		bool is_zero;

		Power()
			: windows(nullptr)
		{

		}

		Power(const char* str)
			: windows(nullptr)
		{
			mpz_t power;
			mpz_init_set_str(power, str, 10);

			size_t number_of_limbs = mpz_size(power);
			const mp_limb_t* limbs = mpz_limbs_read(power);
			Init(limbs, number_of_limbs);
		}

		Power(const mp_limb_t* limbs, unsigned long limb_count)
			: windows(nullptr)
		{
			Init(limbs, limb_count);
		}

		Power(Power&& other)
			: number_of_bits(other.number_of_bits), q(other.q), d(other.d), is_zero(other.is_zero), windows_number(other.windows_number), windows(nullptr)
		{
			if (windows_number != 0)
			{
				windows = (window*)malloc(windows_number * sizeof(window));
				memcpy(windows, other.windows, windows_number * sizeof(window));
			}
		}

		Power(const Power& other)
			: number_of_bits(other.number_of_bits), q(other.q), d(other.d), is_zero(other.is_zero), windows_number(other.windows_number), windows(nullptr)
		{
			if (windows_number != 0)
			{
				windows = (window*)malloc(windows_number * sizeof(window));
				memcpy(windows, other.windows, windows_number * sizeof(window));
			}
		}

		Power& operator=(Power&& other)
		{
			number_of_bits = other.number_of_bits;
			q = other.q;
			d = other.d;
			is_zero = other.is_zero;
			windows_number = other.windows_number;
			windows = nullptr;
			if (windows_number != 0)
			{
				windows = (window*)malloc(windows_number * sizeof(window));
				memcpy(windows, other.windows, windows_number * sizeof(window));
			}
			return *this;
		}

		Power& operator=(const Power& other)
		{
			number_of_bits = other.number_of_bits;
			q = other.q;
			d = other.d;
			is_zero = other.is_zero;
			windows_number = other.windows_number;
			windows = nullptr;
			if (windows_number != 0)
			{
				windows = (window*)malloc(windows_number * sizeof(window));
				memcpy(windows, other.windows, windows_number * sizeof(window));
			}
			return *this;
		}

		~Power()
		{
			if ((windows_number > 0) && (windows != nullptr))
			{
				if (windows != nullptr)
					free(windows);
			}
		}

		template<class Field, void(*SetOne)(typename Field::Elem&), void(*Sqr)(typename Field::Elem&, const typename Field::Elem&), void(*Copy)(typename Field::Elem&, const typename Field::Elem&), void(*Mul)(typename Field::Elem&, const typename Field::Elem&, const typename Field::Elem&)>
		void Exponentiate(typename Field::Elem& result, const typename Field::Elem& e1) const
		{
			if (windows_number == 0)
			{
				SetOne(result);
				return;
			}

			typename Field::Elem e1_square;
			typename Field::Elem* power_table = new typename Field::Elem[1 << d];
			Sqr(e1_square, e1);
			Copy(power_table[1], e1);
			for (unsigned int i = 1; i < 1 << (d - 1); i++)
			{
				Mul(power_table[2 * i + 1], power_table[2 * i - 1], e1_square);
			}
			Copy(result, power_table[windows[windows_number - 1].val]);
			int current_window = windows_number - 2;
			for (int i = 1 + windows[windows_number - 1].pos; i < number_of_bits; i++)
			{
				Sqr(result, result);
				if ((current_window >= 0) && (windows[current_window].pos == i))
				{
					Mul(result, result, power_table[windows[current_window].val]);
					current_window--;
				}
			}
			delete[] power_table;
		}

		template<class Field, void(*SetOne)(typename Field::Elem&), void(*Sqr)(typename Field::Elem&, const typename Field::Elem&), void(*Copy)(typename Field::Elem&, const typename Field::Elem&), void(*Mul)(typename Field::Elem&, const typename Field::Elem&, const typename Field::Elem&)>
		static void MultiExponentiate(typename Field::Elem& result, const typename Field::Elem* elems, const Power* powers, unsigned long long n)
		{
			typename Field::Elem e_square;
			typename Field::Elem** power_table = new typename Field::Elem*[n];
			int* current_windows = new int[n];
			unsigned long long max_number_of_bits = 0;
			unsigned long long* bits_delay = new unsigned long long[n];
			for (unsigned long long i = 0; i < n; i++)
			{
				current_windows[i] = powers[i].windows_number - 1;
				if (powers[i].number_of_bits > max_number_of_bits)
				{
					max_number_of_bits = powers[i].number_of_bits;
				}
				power_table[i] = new typename Field::Elem[1 << (powers[i].d - 1)];
				Sqr(e_square, elems[i]);
				Copy(power_table[i][0], elems[i]);
				for (unsigned int j = 1; j < (1 << (powers[i].d - 1)); j++)
				{
					Mul(power_table[i][j], power_table[i][j - 1], e_square);
				}
			}
			for (unsigned long long i = 0; i < n; i++)
			{
				bits_delay[i] = max_number_of_bits - powers[i].number_of_bits;
			}
			SetOne(result);
			for (int i = 0; i < max_number_of_bits; i++)
			{
				Sqr(result, result);
				for (unsigned long long j = 0; j < n; j++)
				{
					if ((current_windows[j] >= 0) && (powers[j].windows[current_windows[j]].pos + bits_delay[j] == i))
					{
						Mul(result, result, power_table[j][powers[j].windows[current_windows[j]].val >> 1]);
						current_windows[j]--;

					}
				}
			}
			for (unsigned long long i = 0; i < n; i++)
			{
				delete[] power_table[i];
			}
			delete[] power_table;
			delete[] current_windows;
			delete[] bits_delay;
		}

		static void QuickSort(int* index, mpz_t* p_arr, int left, int right)
		{
			int i = left, j = right;
			mpz_t pivot;
			mpz_init_set(pivot, p_arr[index[(left + right) / 2]]);

			while (i <= j)
			{
				while (mpz_cmp(p_arr[index[i]], pivot) > 0)
				{
					i++;
				}
				while (mpz_cmp(p_arr[index[j]], pivot) < 0)
				{
					j--;
				}
				if (i <= j)
				{
					//mpz_swap(p_arr[i], p_arr[j]);
					//swap(e_arr[i], e_arr[j]);
					swap(index[i], index[j]);
					i++;
					j--;
				}
			};

			if (left < j)
			{
				QuickSort(index, p_arr, left, j);
			}
			if (i < right)
			{
				QuickSort(index, p_arr, i, right);
			}
		}

		static int binarySearch(int* index, mpz_t* arr, mpz_t value, int left, int right)
		{
			int middle;
			while (left <= right)
			{
				middle = (left + right) / 2;
				if (mpz_cmp(arr[index[middle]], value) == 0)
				{
					return middle;
				}
				else if (mpz_cmp(arr[index[middle]], value) < 0)
				{
					right = middle - 1;
				}
				else
				{
					left = middle + 1;
				}
			}
			return right + 1;
		}

		template<class Field, void(*SetOne)(typename Field::Elem&), void(*Sqr)(typename Field::Elem&, const typename Field::Elem&), void(*Copy)(typename Field::Elem&, const typename Field::Elem&), void(*Mul)(typename Field::Elem&, const typename Field::Elem&, const typename Field::Elem&)>
		static void MultiExponentiateBC(typename Field::Elem& result, const typename Field::Elem* elems, const Power* powers, unsigned long long n)
		{
			/*char numb[100500];
			ofstream fout("BCLog.txt");*/
			typename Field::Elem tmp;
			mpz_t* p = new mpz_t[n];
			mpz_t q;
			mpz_init(q);
			for (unsigned long long i = 0; i < n; i++)
			{
				mpz_init_set(p[i], powers[i].power);
			}
			typename Field::Elem* e = new typename Field::Elem[n];
			for (unsigned long long i = 0; i < n; i++)
			{
				Copy(e[i], elems[i]);
			}
			int* index = new int[n];
			for (unsigned long long i = 0; i < n; i++)
			{
				index[i] = i;
			}
			QuickSort(index, p, 0, n - 1);
			/*int iter = 0;
			fout << "Iteration " << iter++ << '\n';
			for (unsigned long long i = 0; i < n; i++)
			{
				mpz_get_str(numb, 10, p[index[i]]);
				fout << numb << '\n';
			}
			fout << '\n' << flush;*/
			while (mpz_cmp_ui(p[index[1]], 0) != 0)
			{
				mpz_tdiv_qr(q, p[index[0]], p[index[0]], p[index[1]]);
				if (mpz_cmp_ui(q, 1) == 0)
				{
					Mul(e[index[1]], e[index[1]], e[index[0]]);
				}
				else
				{
					Power tmp_pow(mpz_limbs_read(q), mpz_size(q));
					tmp_pow.Exponentiate<Field, SetOne, Sqr, Copy, Mul>(tmp, e[index[0]]);
					Mul(e[index[1]], e[index[1]], tmp);
				}
				//mpz_sub(p[0], p[0], p[1]);
				int new_pos = binarySearch(index, p, p[index[0]], 1, n - 1);
				int move_val = index[0];
				for (int i = 0; i < new_pos - 1; i++)
				{
					index[i] = index[i + 1];
				}
				index[new_pos - 1] = move_val;

				/*fout << "Iteration " << iter++ << '\n';
				for (unsigned long long i = 0; i < n; i++)
				{
					mpz_get_str(numb, 10, p[index[i]]);
					fout << numb << '\n';
				}
				fout << '\n' << flush;*/
			}
			if (mpz_cmp_ui(p[index[0]], 1) != 0)
			{
				Power tmp_pow(mpz_limbs_read(p[index[0]]), mpz_size(p[index[0]]));
				tmp_pow.Exponentiate<Field, SetOne, Sqr, Copy, Mul>(result, e[index[0]]);
			}
			else
			{
				Copy(result, e[index[0]]);
			}
			for (unsigned long long i = 0; i < n; i++)
			{
				mpz_clear(p[i]);
			}
			delete[] p;
			delete[] e;
			delete[] index;

			/*fout.close();*/
		}
	};

}