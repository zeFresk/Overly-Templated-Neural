#include "stdafx.h"
#include "CppUnitTest.h"

//hack pour faire les tests
#define private public

#include "../Neural-Test/neural.h"

#define private private

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{		
	TEST_CLASS(Constructors)
	{
	public:
		
		TEST_METHOD(OneNoConversion)
		{
			Neural<1> test{ static_cast<float_type>(1) , static_cast<float_type>(1) };
			Assert::AreEqual(test.m_thresold, static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[0], static_cast<float_type>(1));
		}

		TEST_METHOD(OneThrConversion)
		{
			Neural<1> test{ 1 , static_cast<float_type>(1) };
			Assert::AreEqual(test.m_thresold, static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[0], static_cast<float_type>(1));
		}

		TEST_METHOD(OneWeiConversion)
		{
			Neural<1> test{ static_cast<float_type>(1) , 1 };
			Assert::AreEqual(test.m_thresold, static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[0], static_cast<float_type>(1));
		}

		TEST_METHOD(TwoNoConversion)
		{
			Neural<2> test{ static_cast<float_type>(1) , static_cast<float_type>(1) , static_cast<float_type>(1) };
			Assert::AreEqual(test.m_thresold, static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[0], static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[1], static_cast<float_type>(1));
		}

		TEST_METHOD(TwoAllConversions)
		{
			Neural<2> test{ 1 , 1 , -1 };
			Assert::AreEqual(test.m_thresold, static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[0], static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[1], static_cast<float_type>(-1));
		}

		TEST_METHOD(TwoNoConversionMiddle)
		{
			Neural<2> test{ 1 , static_cast<float_type>(1) , 1 };
			Assert::AreEqual(test.m_thresold, static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[0], static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[1], static_cast<float_type>(1));
		}

		TEST_METHOD(ThreeFullConversion)
		{
			Neural<3> test{ 1 , 1 , 1 , 1 };
			Assert::AreEqual(test.m_thresold, static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[0], static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[1], static_cast<float_type>(1));
			Assert::AreEqual(test.m_weights[2], static_cast<float_type>(1));
		}
	};

	TEST_CLASS(Methods)
	{
	public:

		TEST_METHOD(OneCompute)
		{
			Assert::IsTrue(test.compute(2.1));
			Assert::IsFalse(test.compute(2));
			Assert::IsFalse(test.compute(1));
		}

		TEST_METHOD(OneModifyThresold)
		{
			test.modify_thresold(1.5);
			Assert::AreEqual(test.m_thresold, static_cast<float_type>(1.5));
			test.modify_thresold(2);
		}

		TEST_METHOD(OneModifyWeight)
		{
			test.modify_weight(0, 0);
			Assert::AreEqual(test.m_weights[0], static_cast<float_type>(0));
			test.modify_weight(0, 1);
		}

		TEST_METHOD(OneModifyWeights)
		{
			test.modify_weights( 10 );
			Assert::AreEqual(test.m_weights[0], static_cast<float_type>(10));
			test.modify_weights( 1 );
		}

		TEST_METHOD(TwoModifyWeights)
		{
			test2.modify_weights(10, 9);
			Assert::AreEqual(test2.m_weights[0], static_cast<float_type>(10));
			Assert::AreEqual(test2.m_weights[1], static_cast<float_type>(9));
			test2.modify_weights(1, 2);
		}

		TEST_METHOD(ThreeModifyWeights)
		{
			test3.modify_weights(10, 9, 8);
			Assert::AreEqual(test3.m_weights[0], static_cast<float_type>(10));
			Assert::AreEqual(test3.m_weights[1], static_cast<float_type>(9));
			Assert::AreEqual(test3.m_weights[2], static_cast<float_type>(8));
			test3.modify_weights(1, 2, 3);
		}

		TEST_METHOD(TwoCompute)
		{
			Assert::IsTrue(test2.compute(2, 2));
			Assert::IsFalse(test2.compute(1, 1));
			Assert::IsFalse(test2.compute(1, 0.5));
		}

		TEST_METHOD(ThreeCompute)
		{
			Assert::IsTrue(test3.compute(2, 2, 2));
			Assert::IsFalse(test3.compute(1, 1, 1));
			Assert::IsFalse(test3.compute(1, 0.5, 1));
		}

	private:
		Neural<1> test{ 2 , 1 };
		Neural<2> test2{ 3 , 1 , 2 };
		Neural<3> test3{ 6 , 1 , 2 , 3 };
	};
}