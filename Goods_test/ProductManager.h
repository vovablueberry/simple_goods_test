#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <mutex>

#include "types.h"

#define MAX_PRODUCT_COUNT 100000

typedef std::unordered_map<std::string, Product>		productMap;


enum E_PM_RESULT
{
	E_PM_SUCCESS = 0,
	E_PM_FAIL
};

enum E_PM_PRINT_LEVEL	
{
	E_PM_PRINT,			// печтать инфу на экран
	E_PM_SILENT			// не печатать
};

class ProductManager
{
private:
	std::unordered_map<std::string, Product> m_mProductsByID;
	std::multimap<std::string, std::string> m_mProductsByManufacturer;
	
	std::recursive_mutex mtxLock;		// мьютекс защиты для моногпоточного доступа

	E_PM_PRINT_LEVEL prnLevel;

	void init();							
	void releaseData();

	productMap::iterator getProductItem(const std::string& ID);

public:
	ProductManager();
	~ProductManager();

	void setPrintLevel(E_PM_PRINT_LEVEL level)
	{
		this->prnLevel = level;
	};

	// добавить товар в хранилище
	E_PM_RESULT addProduct(const Product& product, const std::string& owner=std::string());
	// удалить товар с заданным артикулом
	E_PM_RESULT removeProduct(const std::string& ID, const std::string& owner=std::string());

	// получить товар с заданным артикулом
	Product* getProductByID(const std::string& ID, const std::string& owner=std::string());
	// получить множество товаров заданного производителя
	std::vector<Product> getProductsByManufacturer(const std::string& Manufacturer, const std::string& owner=std::string());

};

