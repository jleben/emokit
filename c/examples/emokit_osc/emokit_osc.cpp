#include "libepoc.h"

#include <lo/lo.h>

#include <getopt.h>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

bool run = false;

void sigproc(int i)
{
	std::cout << std::endl << "Epoc OSC: Quitting..." << std::endl;
	run = false;
}

typedef basic_string<unsigned char> byte_array;

void parse_key( const std::string & str, byte_array & bytes ) {
	istringstream keystream(str);

	int len = str.length();
	int i=0;
	while(len > 0)
	{
		int n = std::min(2,len);
		istringstream iss(str.substr(i,n));
		int val;
		iss >> hex >> val;
		bytes += static_cast<unsigned char>(val);
		i += n;
		len -= n;
	}
}

struct epoc_osc_options {
	struct {
		byte_array key;
		unsigned int device;
	} epoc;

	struct {
		string addr;
		string port;
	} net;

	struct {
		string prefix;
	} osc;

	bool parse( int argc, char *argv[] )
	{
		//defaults:
		epoc.device = 0;
		osc.prefix = "/epoc";

		struct option long_opts[] = {
			{"address", required_argument, 0, 'a'},
			{"port", required_argument, 0, 'p'},
			{"key", required_argument, 0, 'k'},
			{"device", required_argument, 0, 'd'},
			{"osc-prefix", required_argument, 0, 1}
		};

		int i;
		int c;
		while ((c = getopt_long (argc, argv, "a:p:k:d:", long_opts, &i )) != -1)
		{
			switch (c)
			{
			case 'a':
				net.addr = optarg;
				break;
			case 'p':
				net.port = optarg;
				break;
			case 'k':
				parse_key( optarg, epoc.key);
				break;
			case 'd':
				istringstream(optarg) >> epoc.device;
				break;
			case 1:
				osc.prefix = optarg;
				break;
			case '?':
				return false;
			default:
				return false;
			}
		}

		if( !epoc.key.size() )
			epoc.key.append(CONSUMER4_KEY, KEY_SIZE);

		if( !net.addr.size() ) net.addr = "localhost";
		if( !net.port.size() ) net.port = "57120";

		return true;
	}

	void print() {
		cout << "Options:" << endl;

		cout << "- Epoc device index: " << epoc.device << endl;

		size_t n = epoc.key.size();
		cout << "- Epoc data encryption key: [" << n << "]";
		std::ostream hexstream( cout.rdbuf() );
		hexstream.width(2);
		hexstream.fill('0');
		hexstream.flags( ios::right | ios::hex);
		for(int i=0; i<n; ++i) {
			cout << " ";
			int val = static_cast<int>(epoc.key[i]);
			if( val > 0 )
				hexstream << val;
			else
				cout << "00";
		}
		cout << endl;

		cout << "- Network address: " << net.addr << endl;
		cout << "- Network port: " << net.port << endl;

		cout << "- OSC path prefix: " << osc.prefix << endl;
		cout << endl;
	}
};

int main(int argc, char* argv[])
{
	run = true;

	signal(SIGINT, sigproc);
#ifndef WIN32
	signal(SIGQUIT, sigproc);
#endif

	epoc_osc_options opt;
	if( !opt.parse( argc, argv ) )
		return 1;

	opt.print();

	epoc_device* d;
	uint8_t data[32];
	struct epoc_frame frame;

	int key_len = opt.epoc.key.length();
	unsigned char *key_data = new unsigned char[key_len];
	memcpy( key_data, opt.epoc.key.data(), key_len );

	epoc_init( key_data, key_len );

	d = epoc_create();
	printf("%d Epoc devices connected.\n", epoc_get_count(d, EPOC_VID, EPOC_PID));

	if(epoc_open(d, EPOC_VID, EPOC_PID, opt.epoc.device) != 0)
	{
		printf("Can not attach to Epoc device %d.\n", opt.epoc.device);
		run = false;
	}
	else
	{
		printf("Successfully attached to Epoc device %d.\n", opt.epoc.device);
		printf("Running...\n");
	}

	lo_address addr = lo_address_new( opt.net.addr.c_str(), opt.net.port.c_str() );
	string osc_eeg_addr( opt.osc.prefix + "/eeg" );
	string osc_gyro_addr( opt.osc.prefix + "/gyro" );

	while(run)
	{
		if(epoc_read_data(d, data) > 0)
		{
			epoc_get_next_frame(&frame, data);

			//printf("%d %d %d %d %d\n", frame.gyroX, frame.gyroY, frame.F3, frame.FC6, frame.P7);

			lo_message msg_channels = lo_message_new();
			lo_message_add( msg_channels, "iiiiiiiiiiiiii",
				frame.F3, frame.FC6, frame.P7, frame.T8, frame.F7, frame.F8,
				frame.T7, frame.P8, frame.AF4, frame.F4, frame.AF3, frame.O2,
				frame.O1, frame.FC5 );

			lo_message msg_gyro = lo_message_new();
			lo_message_add( msg_gyro, "ii", (int)frame.gyroX, (int)frame.gyroY );

			lo_bundle bundle = lo_bundle_new( LO_TT_IMMEDIATE );
			lo_bundle_add_message( bundle, osc_eeg_addr.c_str(), msg_channels );
			lo_bundle_add_message( bundle, osc_gyro_addr.c_str(), msg_gyro );

			int res = lo_send_bundle( addr, bundle );
			if( res == -1 ) {
				printf("Sending OSC failed!\n");
				run = false;
			}

			fflush(stdout);
		}
	}

	printf("Releasing Epoc device.\n");

	delete key_data;
	epoc_close(d);
	epoc_delete(d);

	printf("Finished.\n");

	return 0;

}
