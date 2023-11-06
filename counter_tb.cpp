#include "Vcounter.h"          // Header for the 'counter' module generated by Verilator.
#include "verilated.h"         // Basic functionality for Verilator.
#include "verilated_vcd_c.h"   // Header for waveform dump functionality in Verilator.
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    int i; // Variable to count simulation clock cycles.
    int clk; // Variable to toggle the clock signal for the module.
    // Parse and pass the command line arguments to Verilator.
    Verilated::commandArgs(argc, argv);

    // Instantiate the counter module for the Verilator simulation.
    Vcounter* top = new Vcounter;

    // Initialize waveform tracing in Verilator.
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;  // Create a new VCD (Value Change Dump) file object.
    top->trace(tfp, 99);                      // Enable tracing on the counter module.
    tfp->open("counter.vcd");      
    //init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");           // Open the VCD file for writing waveform data.

    // Initialize simulation inputs for the counter module.
    top->clk = 1;     // Set clock to an initial value of 1.
    top->rst = 1;     // Assert the reset signal initially.
    top->en = 0;      // Disable the counter initially.
    //vbdSetMode(1);
    // Run the simulation for 300 clock cycles.
    for (i = 0; i < 1000; i++) {

        // Toggle the clock and dump simulation data into the VCD file.
        for (clk = 0; clk < 2; clk++) {
            tfp->dump(2 * i + clk);     // Write waveform data into the VCD file.
            top->clk = !top->clk;       // Toggle the clock signal.
            top->eval();                // Evaluate the counter module with the new clock value.

        }

        //++++ send count value to VBuddy
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1);

        top->rst = (i<2) | (i==15);
        top->en = vbdFlag();
        if (Verilated::gotFinish())  exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}


