#include<iostream>
#include<string>


class Counter {

private:
	int value;

public:
	Counter() : value(1) {}
	Counter(int initialValue) : value(initialValue) {}
	void increment() {
		value++;
	}

	void decrement() {
		value--;
	}

	int getValue() const {
		return value;
	}
};


int main() {
	std::string answer;
	Counter counter;
	std::cout << "Do you want to specify the initial value of the counter? Enter yes or no: ";
	std::cin >> answer;

	if (answer == "Yes") {
		int initialValue;
		std::cout << "Enter the initial value of the counter: ";
		std::cin >> initialValue;
		counter = Counter(initialValue);
	}

	char command;
	while (true) {
		std::cout << "Enter a command ('+', '-', '=', or 'x'): ";
		std::cin >> command;

		switch (command) {
		case '+':
			counter.increment();
			break;

		case '-':
			counter.decrement();
			break;

		case '=':
			std::cout << counter.getValue() << std::endl;
         
		case 'x':
			std::cout << "Goodbye!" << std::endl;
			EXIT_SUCCESS;

		default:
			std::cout << "Unknown command. Try again." << std::endl;
			break;

		}
	}

	EXIT_SUCCESS;
}






