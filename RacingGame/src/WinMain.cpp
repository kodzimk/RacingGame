#include"StartMenu/Menu.h"
#include"..\src\Others\Win.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Menu* menu = new Menu(1024, 768, hInstance);

	while (menu != nullptr && true){
		menu->ProccesMessages();
	}
	
	return 0;
}