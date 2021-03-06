// Goods_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>

#include "ProductManager.h"

using namespace std;

void workerThread(ProductManager* pMan, uint32_t iThreadID)
{
	if (!pMan)
		return;

	string strThreadName = "thr " + to_string(iThreadID) + " ";

	string strManufac1 = "Airbus";
	string strManufac2 = "Boeing";

	for(int i = 0; i < 10; ++i)
	{
		Product prd( ((i % 2 == 0) ? "A" : "B") + to_string(i),
			(i % 2 == 0) ? strManufac1 : strManufac2,
			string("Tovar: " + to_string(i) + ", thread=" + strThreadName),
			rand());
				

		////////////// ADD
		pMan->addProduct(prd, strThreadName);

		////////////// SEARCH by MAN
		pMan->getProductsByManufacturer(strManufac1, strThreadName);
		pMan->getProductsByManufacturer(strManufac2, strThreadName);


		std::this_thread::sleep_for(std::chrono::seconds(2));
		
		///////////// REMOVE
		pMan->removeProduct(((i % 2 == 0) ? "A" : "B") + to_string(i), strThreadName);

//		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
};

void test_for_collisions(ProductManager* pMan)
{
	if (!pMan)
		return;

	pMan->setPrintLevel(E_PM_PRINT);

	std::thread task1(workerThread, pMan, 1);
	std::thread task2(workerThread, pMan, 2);
	std::thread task3(workerThread, pMan, 3);

	task1.join();
	task2.join();
	task3.join();

	cout << endl;
}

void load_test(ProductManager* pMan)
{
	string strMan1 = "Manufacturer_1";
	string strMan2 = "Manufacturer_2";
	int64_t iMaxObjsCount = 1005000;

	pMan->setPrintLevel(E_PM_SILENT);
	cout << "Adding " << iMaxObjsCount << " objs..." << endl;

	auto startTime = chrono::high_resolution_clock::now();
	for (int64_t i = 0; i < iMaxObjsCount; ++i)
	{
		Product prd(to_string(i),
			(i % 2 == 0) ? strMan1 : strMan2,
			"Product " + to_string(i),
			rand());

		pMan->addProduct(prd);
	}
	auto endTime = chrono::high_resolution_clock::now();
	auto elapsed_1 = endTime - startTime;
	
	cout << "Added "
		<< iMaxObjsCount
		<< "objs, elapsed time="
		<< chrono::duration_cast<chrono::milliseconds>(elapsed_1).count()
		<< "ms,  "
		<< chrono::duration_cast<chrono::microseconds>(elapsed_1).count()
		<< "us, "
		<< chrono::duration_cast<chrono::nanoseconds>(elapsed_1).count()
		<< "ns"
		<< endl << endl; 
	//////////////////////////////////////////////////////////////////
	string strIDF = to_string(iMaxObjsCount - 1);

	cout << "Finding object by ID=" << strIDF << "..." << endl;
	pMan->setPrintLevel(E_PM_PRINT);
	auto startTime2 = chrono::high_resolution_clock::now();
	Product* pPrd = pMan->getProductByID(strIDF);
	auto endTime2 = chrono::high_resolution_clock::now();
	auto elapsed_2 = endTime2 - startTime2;
	
	cout << "End finding obj, elapsed time="
		<< chrono::duration_cast<chrono::milliseconds>(elapsed_2).count()
		<< "ms,  "	
		<< chrono::duration_cast<chrono::microseconds>(elapsed_2).count()
		<< "us, "
		<< chrono::duration_cast<chrono::nanoseconds>(elapsed_2).count()
		<< "ns"
		<< endl;
}


int main()
{
	ProductManager* pMan = new ProductManager;
	char cCommand;
	bool bReadyToStop = false;

	while (!bReadyToStop)
	{
		cout << endl << "Choose command: " << endl
			<< "1 - test for collisions," << endl
			<< "2 - load test" << endl
			<< "> ";

		cin >> cCommand;

		switch (cCommand)
		{
		case '1':
			test_for_collisions(pMan);
			break;
		case '2':
			load_test(pMan);
			break;

		default:
			bReadyToStop = true;
			break;
		}
	}

	
	delete pMan;

	system("pause");

    return 0;
}

