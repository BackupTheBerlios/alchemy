#ifndef SAMPLESATPARAMS_H_MAR_21_2005
#define SAMPLESATPARAMS_H_MAR_21_2005

struct SampleSatParams
{
  int saRatio;		// percent of SA steps
  int temperature;	// temperature/100: SA temperature
  bool latesa;		// sa only at a plateur
  int numsol;		// the nth solution to return
  int ws_noise;		// ws noise
  int ws_restart;	// ws restart = numtry
  int ws_cutoff;	// ws cutoff
};

#endif
