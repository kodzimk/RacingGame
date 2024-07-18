#include"StartMenu/Menu.h"
#include"..\src\Others\Win.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Menu* menu = new Menu(1024, 768, hInstance);

	while (menu != nullptr) {
		if (const auto ecode = menu->ProccesMessages())
		{
			if (ecode == 1)
				return 1;
			else if (ecode == 2)
				return 2;
		}
	}
	
	return 0;
}