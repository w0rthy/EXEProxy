#include <cstdlib>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char* argv[]){
	FreeConsole();
	//LAUNCH PROXIED EXECUTABLE
	SHELLEXECUTEINFO sei;
	struct stat buffer;
	if(stat("Proxy.exe", &buffer) == 0){
		
		int size = 1;
		for(int i = 1; i < argc; i++)
			size+=strlen(argv[i])+3;
		char* args = (char*)malloc(size);
		args[0]='\0';
		for(int i = 1; i < argc; i++){
			strcat_s(args,size,"\"");
			strcat_s(args,size,argv[i]);
			strcat_s(args,size,"\" ");
		}
		args[size-1]='\0';

		ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));
		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		sei.lpVerb = "open";
		sei.lpFile = "Proxy.exe";
		sei.nShow = SW_SHOWDEFAULT;
		sei.lpParameters = args;
		//ShellExecute(NULL, "open", "Proxy.exe", args, NULL, SW_SHOWDEFAULT);
		ShellExecuteEx(&sei);
		free(args);
	}
	else{
		cout<<"Proxy.exe not found; please verify a valid executable exists in the same directory";
		return 0;
	}
	//SAVE INTERCEPTED ARGUMENTS
	ofstream str;
	str.open("EXEProxy_output.txt");

	for(int i = 0; i < argc; i++)
		str<<argv[i]<<'\n';

	str.close();
	
	if(sei.hProcess){
		WaitForSingleObject(sei.hProcess,INFINITE);
		CloseHandle(sei.hwnd);
		CloseHandle(sei.hProcess);
	}
}