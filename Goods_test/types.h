#pragma once

#include <string>

//---------------------------
//   "�����"
//---------------------------
struct Product
{
	std::string ID;					// �������
	std::string Manufacturer;		// �������������
	std::string Name;				// ������������ ������
	uint32_t	Price;				// ����
		

	Product() : ID(),
		Manufacturer(),
		Name(),
		Price(0) {};

	Product(const std::string& sID, const std::string& sMan, const std::string& sName, const uint32_t& iPrice)
	{
		this->ID = sID;
		this->Manufacturer = sMan;
		this->Name = sName;
		this->Price = iPrice;
	};
		
	Product(const Product& src)
	{
		this->ID = src.ID;
		this->Manufacturer = src.Manufacturer;
		this->Name = src.Name;
		this->Price = src.Price;
	};

	Product& operator=(const Product& src)
	{
		this->ID = src.ID;
		this->Manufacturer = src.Manufacturer;
		this->Name = src.Name;
		this->Price = src.Price;

		return *this;
	};
};