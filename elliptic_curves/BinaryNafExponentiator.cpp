//#include "BinaryNafExponentiator.h"
//
//
//
//BinaryNafExponentiator::BinaryNafExponentiator(const BigInt& biPower)
//{
//	m_vNaf = biPower.GetNaf();
//}
//
//
//BinaryNafExponentiator::~BinaryNafExponentiator()
//{
//}
//
//void BinaryNafExponentiator::Exponentiate(const Field& field, const FieldElem & elemToExp, FieldElem & result) const
//{
//	result = field.GetOneElement();
//	for (auto it = m_vNaf.begin(); it != m_vNaf.end(); it++)
//	{
//		if (*it == 1)
//		{
//			field.AddAssign(result, elemToExp);
//		}
//		else if (*it == -1)
//		{
//			field.SubtractAssign(result, elemToExp);
//		}
//		field.SquareAssign(result); // TODO! Implement squaring
//	}
//}
