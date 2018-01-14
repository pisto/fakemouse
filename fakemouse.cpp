#include <cstdio>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <csignal>
#include <unistd.h>
#include <fcntl.h>
#include <linux/uinput.h>

using namespace std;

int main(int argc, char** argv) try {
	if(argc < 2) throw invalid_argument("please specify a uinput device file");
	int vmouse = open(argv[1], O_WRONLY);
	if(vmouse == -1) throw "open uinput file";
	for(auto ev: { EV_REL, EV_KEY, EV_ABS, EV_SYN }) if(ioctl(vmouse, UI_SET_EVBIT, ev)) throw "UI_SET_EVBIT";
	for(auto key: { BTN_LEFT, BTN_RIGHT }) if(ioctl(vmouse, UI_SET_KEYBIT, key)) throw "UI_SET_KEYBIT";
	for(auto rel: { REL_X, REL_Y }) if(ioctl(vmouse, UI_SET_RELBIT, rel)) throw "UI_SET_RELBIT";
	uinput_user_dev uidev;
	memset(&uidev, 0, sizeof(uidev));
	strncpy(uidev.name, "fakemouse", UINPUT_MAX_NAME_SIZE);
	uidev.id.bustype = BUS_VIRTUAL;
	if(write(vmouse, &uidev, sizeof(uidev)) != sizeof(uidev)) throw "writing device specification";
	if(ioctl(vmouse, UI_DEV_CREATE)) throw "UI_DEV_CREATE";
	pause();
} catch(const char* at){
	throw runtime_error(string(at) + " (" + strerror(errno) + ")");
}
