#include <iostream>
#include "Application.h"
//#include <time.h>
//#pragma pack(4)

class LayoutModel{
public:
	LayoutModel() { m_x = 1; m_y = 2; }
	virtual ~LayoutModel() {}
	//int m_z;
	short m_x;
	long long m_y;
};

int main(int argc, char* argv[])
{
	Application app;
	app.Run();

	LayoutModel *pModel = new LayoutModel;
	char *p = nullptr;
	printf("Sizeof(model)=%d\n", (int)sizeof(*pModel));

	printf("Fxkk========>>>Done %d", 2&64);
	std::cin.get();

	return 0;
}
