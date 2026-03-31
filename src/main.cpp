#include <iostream>
#include "solution.h"

constexpr int max_bound = 1e9 + 7;

void ConstructorTest() {
	std::cout << "======== Constructor Test ========\n";
	std::vector node_bounds({10, 20, 30, 40, 50, 60, 70, 80, 90, 97});
	SpeedCircularLinkedList<int, 97> speed_circular_linked_list(node_bounds);
	speed_circular_linked_list.print();
}

void SizeTest() {
	std::cout << "============ Size Test ============\n";
	std::vector node_bounds({10, 30, 40, 50, 53});
	SpeedCircularLinkedList<int, 53> speed_circular_linked_list(node_bounds);
	std::cout << "size = " << speed_circular_linked_list.size() << '\n';
}

void PutTest() {
	std::cout << "============ Put Test ============\n";
	std::vector node_bounds({10, 30, 50, 70, 90, 110, 130, 150, 170, 197});
	SpeedCircularLinkedList<int, 197> speed_circular_linked_list(node_bounds);
	speed_circular_linked_list.put("19260817", 0); // Store to the node with bound = 150;
	speed_circular_linked_list.put("What a pity!", 1); // Store to the node with bound = 150;
	speed_circular_linked_list.put("ACM2025", 6); // Store to the node with bound = 50;
	speed_circular_linked_list.print();
}

void GetTest() {
	std::cout << "=========== Get Test =============\n";
	std::vector node_bounds({50, 250, 450, 800, 1000, 1300, 1350, 1600, 1850, 2017});
	SpeedCircularLinkedList<std::string, 2017> malaysia_west_coast_railway(node_bounds);
	malaysia_west_coast_railway.put("Butterworth", "07:50");
	malaysia_west_coast_railway.put("Ipoh", "09:37");
	malaysia_west_coast_railway.put("KL Sentral", "12:15");
	malaysia_west_coast_railway.put("Seremban", "13:38");
	malaysia_west_coast_railway.put("Gemas", "14:42");
	malaysia_west_coast_railway.put("Segamat", "14:57");
	std::cout << malaysia_west_coast_railway.get("KL Sentral") << '\n'; // 12:15
	std::cout << malaysia_west_coast_railway.get("Gemas") << '\n'; // 14:42
	std::cout << malaysia_west_coast_railway.get("Butterworth") << '\n'; // 07:50
}

void YourTest() {
	// You can add your personal test code as you like.
}

int main() {
	ConstructorTest();
	SizeTest();
	PutTest();
	GetTest();
	YourTest();
    return 0;
}