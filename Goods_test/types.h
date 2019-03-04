#pragma once

#include <string>

//---------------------------
//   "Товар"
//---------------------------
struct Product
{
	std::string ID;					// артикул
	std::string Manufacturer;		// производитель
	std::string Name;				// наименование товара
	uint32_t	Price;				// цена
		

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