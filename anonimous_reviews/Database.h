#pragma once
#include "sqlite3.h"
#include <vector>

#include "Utils.h"

using namespace std;

class Database
{
private:
	static sqlite3* db;

public:

	struct Mtroot
	{
		string m_root;

		Mtroot(string root);
		bool Save();
		bool Delete();
		static bool CheckExtsts(string root);
	};

	struct Store
	{
		string m_id;
		string m_descr;
		string m_pub_key;
		string m_data_hash;

		Store();
		Store(string id, string descr, string pub_key, string data_hash);
		bool Save();
		static bool Load(string id, Store& result);
		static bool LoadAll(vector<Store>& result);
		static bool Find(string descr, vector<Store>& result);
		bool Delete();
	};

	struct Item
	{
		string m_id;
		string m_store_id;
		string m_descr;
		string m_data_hash;

		Item();
		Item(string id, string store_id, string descr, string data_hash);
		bool Save();
		static bool Load(string id, Item& result);
		static bool LoadAll(string store_id, vector<Item>& result);
		static bool Find(string descr, vector<Item>& result);
		bool Delete();
	};

	struct User
	{
		string m_id;
		int m_ordinal;

		User(string id, int ordinal);
		bool Save();
		static bool Load(string id, User& result);
		bool Delete();
	};

	struct Comment
	{
		string m_id;
		string m_item_id;
		string m_mt_root;
		string m_proof;
		string m_data_hash;

		Comment(string id, string item_id, string mt_root, string proof, string data_hash);
		bool Save();
		static bool CheckExists(string id);
		static bool Load(string id, Comment& result);
		static bool LoadAll(string item_id, vector<Comment>& result);
		bool Delete();
	};

	static void Init(string filename);
};

