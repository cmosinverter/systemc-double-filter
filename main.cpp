#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "DoubleFilter.h"
#include "Testbench.h"

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

// int main(int argc, char *argv[])
int sc_main(int argc, char **argv) {
  if ((argc < 3) || (argc > 4)) {
    cout << "No arguments for the executable : " << argv[0] << endl;
    cout << "Usage : >" << argv[0] << " in_image_file_name out_image_file_name"
         << endl;
    return 0;
  }

  //Create modules and signals
  Testbench tb("tb");
  DoubleFilter double_filter("double_filter");
  sc_clock clk("clk", CLOCK_PERIOD, SC_NS);
  sc_signal<bool> rst("rst");

  //Create FIFO channels
  sc_fifo<unsigned char> r;
  sc_fifo<unsigned char> g;
  sc_fifo<unsigned char> b;
  sc_fifo<unsigned char> f_r;
  sc_fifo<unsigned char> f_g;
  sc_fifo<unsigned char> f_b;

  //Connect FIFO channels with modules
  tb.i_clk(clk);
  tb.o_rst(rst);
  double_filter.i_clk(clk);
  double_filter.i_rst(rst);
  tb.o_r(r);
  tb.o_g(g);
  tb.o_b(b);
  tb.i_r(f_r);
  tb.i_g(f_g);
  tb.i_b(f_b);
  double_filter.i_r(r);
  double_filter.i_g(g);
  double_filter.i_b(b);
  double_filter.o_r(f_r);
  double_filter.o_g(f_g);
  double_filter.o_b(f_b);

  tb.read_bmp(argv[1]);
  sc_start();
  std::cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
  tb.write_bmp(argv[2]);

  return 0;
}
