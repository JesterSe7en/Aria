#include <Aria.h>


class Sandbox : public Aria::Application {
public:
	Sandbox() {};
	~Sandbox() {};
};


int main() {
	Sandbox* sandbox = new Sandbox();
	sandbox->Run();
	delete sandbox;
	return 0;
}