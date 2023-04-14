#include <Aria.h>


class Sandbox : public Aria::Application {
public:
	Sandbox() {};
	~Sandbox() {};
};


Aria::Application* Aria::CreateApplication() {
	return new Sandbox();
}