#include"StartMenu/Menu.h"
#include"Level/Windows.h"
#include"..\src\Others\Win.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Menu* menu = new Menu(1024, 768, hInstance);
	Window* window = nullptr;
	int result = 0;

	std::optional<int> ecode = {0};

	while (menu != nullptr || window != nullptr) {
		while (ecode != 1 && ecode != 2)
		{
			    ecode = menu->ProccesMessages();
				if (ecode == 1)
				{
					delete menu;
					menu = nullptr;
					window = new Window(1024,768,hInstance);
				}
				else if (ecode == 2)
				{
					delete menu;
					menu = nullptr;
					result = 2;
			
				}
		}
		while (window != nullptr)
		{
			window->ProccesMessages();
			window->InputHandler();
			if(window->ecode == 2)
			{
				result = window->ecode;
				delete window;
				window = nullptr;
			}
		}
	}
	

	return result;
}