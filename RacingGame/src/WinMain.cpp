#include"StartMenu/Menu.h"
#include"..\src\Others\Win.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Menu* menu = new Menu(800, 600, hInstance);

	while (menu != nullptr && true){
		while (menu != nullptr){	
			if (const auto ecode = menu->ProccesMessages())
			{
				if (ecode == 0)
					menu = nullptr;

				return *ecode;
			}
			menu->gfx->Update();
		}

	}
	
	return 0;
}