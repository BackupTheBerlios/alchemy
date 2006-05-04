#ifndef GELMANCONVERGENCETEST_H_SEP_30_2005
#define GELMANCONVERGENCETEST_H_SEP_30_2005

#include "meanvariance.h"


class GelmanConvergenceTest
{
 public:
  GelmanConvergenceTest(const int& numChains) : numChains_(numChains), 
    withinChainMeanVars_(new MeanVariance[numChains]), numSamples_(0) {}

  ~GelmanConvergenceTest() { delete [] withinChainMeanVars_; }


    // values is array of size numChains_
  void appendNewValues(const double* const & values) 
  {
    for (int i = 0; i < numChains_; i++)
      withinChainMeanVars_[i].appendValue(values[i]);
    numSamples_++;
  }

    // values is array of size numChains_
  void appendNewValues(const bool* const & values) 
  {
    for (int i = 0; i < numChains_; i++)
      withinChainMeanVars_[i].appendValue((double)values[i]);
    numSamples_++;
  }


  double getConvergenceScore()
  {
    betweenChainsMeanVar_.reset();
    double totalW = 0;
    for (int i = 0; i < numChains_; i++) 
    {
      betweenChainsMeanVar_.appendValue( withinChainMeanVars_[i].getMean() );
      totalW += withinChainMeanVars_[i].getVariance();
    }
    int numValues = withinChainMeanVars_[0].getNumValues();

    double B = betweenChainsMeanVar_.getVariance() * numValues;
    double W = totalW / numChains_;

      // score as stated in "Probability and Statistics", DeGroot and Schervish
    double score = B/W;
    return score; 
  }
  

  int getNumSamplesAdded() { return numSamples_; }


  static bool checkConvergenceOfAll(GelmanConvergenceTest* tests[], 
                                    const int& numTests,
                                    const bool& print=false) 
  {
    // threshold as stated in "Probability and Statistics", DeGroot & Schervish 
    double threshold = 1 + 0.44 * tests[0]->getNumSamplesAdded();
    int maxItem     = -1;
    double maxScore = -1;
    int numbad      = 0;

    for (int f = 0; f < numTests; f++) 
    {
      double score = tests[f]->getConvergenceScore();

      if (!finite(score)) { numbad++; continue; }

      if (score > threshold) 
      {
        if (print)
          cout << " Item " << f << "'s score of " << score << " > threshold of "
               << threshold << endl; 
        return false;
      }

      if (score > maxScore) 
      {
        maxScore = score;
        maxItem = f;
      }
    }

    if (numbad == numTests)
    {
      if (print) cout << " All scores were inf or Nan!" << endl;
      return false;
    }
  
    // at this point all scores are less than the threshold

    if (print)
      cout << " max item is " << maxItem << " with score " << maxScore 
           << " < threshold of " << threshold << endl; 

    return true;
  }


 private:  
  int numChains_;
  MeanVariance* withinChainMeanVars_;
  int numSamples_;
  MeanVariance betweenChainsMeanVar_;

};


#endif
