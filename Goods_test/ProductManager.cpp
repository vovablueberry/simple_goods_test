#include "stdafx.h"
#include "ProductManager.h"

using namespace std;

ProductManager::ProductManager()
{
	init();
}
ProductManager::~ProductManager()
{
	releaseData();
}
//-----------------------------------------------------------------------------------------------------
void ProductManager::init()
{
	setPrintLevel(E_PM_SILENT);
	printf("ProductManager::init()\n");
}
void ProductManager::releaseData()
{
	// useless
}	
//-----------------------------------------------------------------------------------------------------
E_PM_RESULT ProductManager::addProduct(const Product& product, const std::string& owner)
{	
	E_PM_RESULT res = E_PM_FAIL;

	//std::lock_guard<std::mutex> guard(mtxLock);
	mtxLock.lock();
	productMap::iterator it = getProductItem(product.ID);
	if (it == m_mProductsByID.end())
	{
		///////////////////////////////////////
		pair<productMap::iterator, bool> addResult = m_mProductsByID.insert(make_pair(product.ID, product));
		bool bAddResult = addResult.second;
		if (addResult.first == m_mProductsByID.end() || !bAddResult)
		{
			if (prnLevel == E_PM_PRINT)
				printf("[%s] addProduct(): FAILED adding Product with ID [%s]!\n", owner.c_str(), product.ID.c_str());			
		}
		else
		{
			m_mProductsByManufacturer.insert(make_pair(product.Manufacturer, product.ID));

			if(prnLevel == E_PM_PRINT)
				printf("[%s] addProduct(): ADDED Product with ID [%s]\n", owner.c_str(), product.ID.c_str());
		
			res = E_PM_SUCCESS;
		}
	}
	else
	{
		if (prnLevel == E_PM_PRINT)
			printf("[%s] addProduct(): Product with ID [%s] already exists!\n", owner.c_str(), product.ID.c_str());
		res = E_PM_FAIL;
	}

	mtxLock.unlock();
		
	return res;
}

//-----------------------------------------------------------------------------------------------------
E_PM_RESULT ProductManager::removeProduct(const std::string& ID, const std::string& owner)
{
	E_PM_RESULT res = E_PM_FAIL;

	//std::lock_guard<std::mutex> guard(mtxLock);
	mtxLock.lock();
	
	productMap::iterator itPrd = getProductItem(ID);
	if (itPrd != m_mProductsByID.end())
	{
		// erase from additional container
		multimap<string, string>::iterator itMan = m_mProductsByManufacturer.begin();
		while (itMan != m_mProductsByManufacturer.end())
		{
			if (itMan->second == ID)
				break;

			++itMan;
		}
		if (itMan != m_mProductsByManufacturer.end())
			m_mProductsByManufacturer.erase(itMan);

		// erase from main container
		m_mProductsByID.erase(itPrd);

		if (prnLevel == E_PM_PRINT)
			printf("[%s] removeProduct(): REMOVED Product with ID [%s]\n", owner.c_str(), ID.c_str());
		
		res = E_PM_SUCCESS;
	}	
	else
	{
		if (prnLevel == E_PM_PRINT)
			printf("[%s] removeProduct(): Product with ID [%s] not found\n", owner.c_str(), ID.c_str());
		res = E_PM_FAIL;
	}

	mtxLock.unlock();

	return res;
}
//-----------------------------------------------------------------------------------------------------
Product* ProductManager::getProductByID(const std::string& ID, const std::string& owner)
{
	Product* res = nullptr;

	//std::lock_guard<std::mutex> guard(mtxLock);
	mtxLock.lock();

	productMap::iterator it = getProductItem(ID);
	if (it != m_mProductsByID.end())
	{
		res = &it->second;
	}
	
	mtxLock.unlock();

	return res;
}
//-----------------------------------------------------------------------------------------------------
productMap::iterator ProductManager::getProductItem(const std::string& ID)
{	
	return m_mProductsByID.find(ID);
}
//-----------------------------------------------------------------------------------------------------
std::vector<Product> ProductManager::getProductsByManufacturer(const std::string& Manufacturer, const std::string& owner)
{
	std::vector<Product> vRes;

	//std::lock_guard<std::mutex> guard(mtxLock);
	mtxLock.lock();

	auto manScope = m_mProductsByManufacturer.equal_range(Manufacturer);
	for (auto it_man = manScope.first; it_man != manScope.second; ++it_man)
	{
		auto prd = getProductItem(it_man->second);
		if (prd != m_mProductsByID.end())
		{
			vRes.push_back(prd->second);
			if (prnLevel == E_PM_PRINT)
				printf("[%s] getProductsByManufacturer(): Manufacturer [%s]: Got ID [%s]\n", 
									owner.c_str(), Manufacturer.c_str(), prd->second.ID.c_str());
		}
	}

	if (vRes.size() == 0)
	{
		if (prnLevel == E_PM_PRINT)
			printf("[%s] getProductsByManufacturer(): Manufacturer [%s]: nothing found.\n", owner.c_str(), Manufacturer.c_str());
	}	

	mtxLock.unlock();	

	return vRes;
}