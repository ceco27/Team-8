#include "PCStore.h"
#include <iostream>
PCStore::PCStore()
{
	components = new Component*[8];
	countComponents = 0;
	capacityComponents = 8;
	money = 0.0;
}

PCStore::PCStore(const PCStore& other)
{
	copy(other);
}

PCStore& PCStore::operator=(const PCStore& other)
{
	if (this != &other)
	{
		free();
		copy(other);
	}

	return *this;
}

PCStore::~PCStore()
{
	free();
}

void PCStore::setMoney(double money)
{
	this->money = money;
}

bool PCStore::checkBalance(double price)
{
	return money > price;
}

const double PCStore::getMoney() const
{
	return money;
}

void PCStore::subtractFunds(double price)
{
	money -= price;
}

void PCStore::addFunds(double price)
{
	money += price * 1.1;
}

void PCStore::printComponentInfos() const
{
	for (size_t i = 0; i < countComponents; i++)
	{
		std::cout << "Component " << i + 1 << ": ";
		components[i]->showComponentInfo();
		std::cout << std::endl;
	}
}

void PCStore::addMotherboard(Motherboard motherboard)
{
	if (countComponents == capacityComponents)
		resize();

	components[countComponents++] = new Motherboard(motherboard);
}

void PCStore::addCpu(Cpu cpu)
{
	if (countComponents == capacityComponents)
		resize();

	components[countComponents++] = new Cpu(cpu);
}

void PCStore::addGpu(Gpu gpu)
{
	if (countComponents == capacityComponents)
		resize();

	components[countComponents++] = new Gpu(gpu);
}

void PCStore::addRam(Ram ram)
{
	if (countComponents == capacityComponents)
		resize();

	components[countComponents++] = new Ram(ram);
}

void PCStore::sellComponent(size_t index)
{
	money += components[index]->getPrice();
	removeFromIndex(index);
	std::cout << "Component sold!" << std::endl;
}

void PCStore::removeFromIndex(size_t index)
{
	if (index >= countComponents)
	{
		std::cout << "No such index!" << std::endl;
		return;
	}

	delete[] components[index];

	for (size_t j = index; j < countComponents - 1; j++)
		components[j] = components[j + 1]->clone();

	if (index != countComponents - 1)
		delete[] components[countComponents - 1];
	countComponents--;
}

void PCStore::buildComputer()
{
	int indexMotherboard, indexCPU, indexGPU, indexRAM;
	std::cout << "Enter index of Motherboard: ";
	std::cin >> indexMotherboard;
	if (components[indexMotherboard - 1]->getComponentType() != MOTHERBOARD)
	{
		std::cout << "This component is not a Motherboard!" << std::endl;
		return;
	}
	std::cout << "Enter index of CPU: ";
	std::cin >> indexCPU;
	std::cout << components[indexCPU - 1]->getComponentType();
	if (components[indexCPU - 1]->getComponentType() != CPU)
	{
		std::cout << "This component is not a CPU!" << std::endl;
		return;
	}
	std::cout << "Enter index of GPU: ";
	std::cin >> indexGPU;
	if (components[indexGPU - 1]->getComponentType() != GPU)
	{
		std::cout << "This component is not a GPU!" << std::endl;
		return;
	}
	std::cout << "Enter index of RAM: ";
	std::cin >> indexRAM;
	if (components[indexRAM - 1]->getComponentType() != RAM)
	{
		std::cout << "This component is not a RAM!" << std::endl;
		return;
	}

	Motherboard* mb = dynamic_cast<Motherboard*>(components[indexMotherboard - 1]);

	if (checkCompatibilityCpu(*dynamic_cast<Motherboard*>(components[indexMotherboard - 1]), *dynamic_cast<Cpu*>(components[indexCPU - 1]))
		&& checkCompatibilityRam(*dynamic_cast<Motherboard*>(components[indexMotherboard - 1]), *dynamic_cast<Ram*>(components[indexRAM - 1])))
	{
		std::cout << "Computer sold!" << std::endl;
		money += components[indexMotherboard - 1]->getPrice() * 1.2;
		money += components[indexCPU - 1]->getPrice() * 1.2;
		money += components[indexGPU - 1]->getPrice() * 1.2;
		money += components[indexRAM - 1]->getPrice() * 1.2;

		int indexesBefore = 0;

		removeFromIndex(indexMotherboard - 1);
		indexesBefore = (indexMotherboard < indexCPU);

		removeFromIndex(indexCPU - 1 - indexesBefore);
		indexesBefore = (indexMotherboard < indexGPU) + (indexCPU < indexGPU);

		removeFromIndex(indexGPU - 1 - indexesBefore);
		indexesBefore = (indexMotherboard < indexRAM) + (indexCPU < indexRAM) + (indexCPU < indexRAM);

		removeFromIndex(indexRAM - 1 - indexesBefore);
	}
	else
	{
		std::cout << "Incompatible components!" << std::endl;
	}
}

bool PCStore::checkCompatibilityCpu(Motherboard& motherboard, Cpu& cpu)
{
	return motherboard.getSocket() == cpu.getSocket();
}

bool PCStore::checkCompatibilityRam(Motherboard& motherboard, Ram& ram)
{
	return motherboard.getRamType() == ram.getRamType();
}

void PCStore::writeToFile() const
{
	std::ofstream file;

	file.open("components.txt");

	file << "money " << money << std::endl;

	for (size_t i = 0; i < countComponents; i++)
		components[i]->saveToFile(file);

	file << "End" << std::endl;

	file.close();
}

void PCStore::copy(const PCStore& other)
{
	components = new Component * [other.capacityComponents];

	countComponents = other.countComponents;
	capacityComponents = other.capacityComponents;

	money = other.money;

	for (size_t i = 0; i < countComponents; i++)
		components[i] = other.components[i]->clone();

}

void PCStore::free()
{
	for (size_t i = 0; i < countComponents; i++)
		delete components[i];

	delete[] components;
}

void PCStore::resize()
{
	capacityComponents *= 2;
	Component** newComponents = new Component * [capacityComponents];

	for (size_t i = 0; i < countComponents; i++)
		newComponents[i] = components[i]->clone();

	for (size_t i = 0; i < countComponents; i++)
		delete components[i];

	delete[] components;

	components = newComponents;
}
