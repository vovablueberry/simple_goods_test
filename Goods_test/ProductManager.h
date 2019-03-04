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
	E_PM_PRINT,			// ������� ���� �� �����
	E_PM_SILENT			// �� ��������
};

class ProductManager
{
private:
	std::unordered_map<std::string, Product> m_mProductsByID;
	std::multimap<std::string, std::string> m_mProductsByManufacturer;
	
	std::recursive_mutex mtxLock;		// ������� ������ ��� �������������� �������

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

	// �������� ����� � ���������
	E_PM_RESULT addProduct(const Product& product, const std::string& owner=std::string());
	// ������� ����� � �������� ���������
	E_PM_RESULT removeProduct(const std::string& ID, const std::string& owner=std::string());

	// �������� ����� � �������� ���������
	Product* getProductByID(const std::string& ID, const std::string& owner=std::string());
	// �������� ��������� ������� ��������� �������������
	std::vector<Product> getProductsByManufacturer(const std::string& Manufacturer, const std::string& owner=std::string());

};

