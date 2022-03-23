#include <Rcpp.h>
using namespace Rcpp;

//Implements functions from https://en.wikipedia.org/wiki/Smooth_maximum

// [[Rcpp::export]]
double S_alpha(NumericVector x, double alpha){
  double scale = max(abs(x));
  x = x/scale;
  double denom = 0;
  double num = 0;
  for(int ii = 0; ii<x.length();ii++){
    double xi=x[ii];
    denom+=exp(alpha*xi);
    num+=xi*exp(alpha*xi);
  }
  return num/denom*scale;
}

// [[Rcpp::export]]
//if alpha is negative, we return a logsumexp like function which gives the min,
//according to:
//https://math.stackexchange.com/questions/3233404/log-sum-exp-as-an-approximation-of-min-function
double LogSumExp(NumericVector x, double alpha){
  if(alpha<0){
    x=-x*log(1.0/x.length());
  }
  double scale = max(abs(x));
  x = x/scale;
  double arg = 0;
  for(int ii = 0; ii<x.length();ii++){
    double xi=x[ii];
    arg+=exp(alpha*xi);
  }
  return 1/alpha*log(arg)*scale;
}

//returns the max of abs(x)
// [[Rcpp::export]]
double p_norm(NumericVector x, double pp){
  double scale = max(abs(x));
  x = x/scale;
  x = abs(x);
  double arg = 0;
  for(int ii = 0; ii<x.length();ii++){
    double xi=x[ii];
    arg+=pow(xi,pp);
  }
  return pow(arg,1.0/pp)*scale;
}

// [[Rcpp::export]]
double smooth_max(NumericVector x, double alpha = 10){
  if(alpha < 0){
    warning("Warning: alpha < 0 returns minimum");
  }
  return S_alpha(x,alpha);
}

// [[Rcpp::export]]
double smooth_min(NumericVector x, double alpha = 10){
  if(alpha < 0){
    warning("Warning: alpha < 0 returns maximum");
  }
  return S_alpha(x,-alpha);
}

// [[Rcpp::export]]
NumericVector smooth_range(NumericVector x, double alpha = 10){
  if(alpha < 0){
    warning("Warning: alpha < 0 returns max then min");
  }
  double min = smooth_min(clone(x),alpha);
  double max = smooth_max(clone(x),alpha);
  NumericVector range = {min,max};
  return range;
}

// 
// /*** R
// S_alpha(c(1,2,3,100),-10)#min
// S_alpha(c(1,2,3,100),0)#mean
// S_alpha(c(1,2,3,100),10)#max
// 
// LogSumExp(c(1,2,3,100),-10)#min
// LogSumExp(c(1,2,3,100),10)#max
// 
// p_norm(c(1,2,3,100),100)#max abs
// p_norm(c(1,2,3,7),10)#max abs
// 
// smooth_min(c(1,2,3,100),10)#range
// smooth_max(c(1,2,3,100),10)#range
// smooth_range(c(1,2,3,100),10)#range
// */
