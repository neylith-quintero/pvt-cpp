#ifndef SPIVEY_DRY_IMPL_H
#define SPIVEY_DRY_IMPL_H

#include <correlations/correlation.H>

inline Quantity<mPascal>
PvSpiveyMN::impl(const Quantity<Kelvin> & tk) const
{
  const double raw_tk = tk.raw();

  constexpr double tc = 647.096; // °K
  constexpr double pc = 22.064; // MPa

  const double v = 1 - (raw_tk/tc);
  const double v2 = v*v;
  const double v3 = v2*v;
  const double v4 = v3*v;
        
  const double lnpv = ((tc/raw_tk) * (-7.85951783*v + 1.84408259*pow(v, 1.5) +
				  -11.7866497*v3 + 22.6807411*pow(v, 3.5) +
				  -15.9618719*v4 + 1.80122502 * pow(v, 7.5))) + log(pc);

  const double pv = exp(lnpv);

  return Quantity<mPascal>(pv);
}

inline Quantity<Molality_NaCl>
MwSpiveyMN::impl(const Quantity<Celsius> & t,
		  const Quantity<mPascal> & p,
		  const Quantity<mPascal> & pv,
		  const Quantity<Molality_NaCl> & nacl_concentration) const
{
  const double t_100 = t.raw()/100;
  const double t_100_2 = t_100*t_100;

  const double a = -0.007751*t_100_2 + 0.013624*t_100 + -0.0781;
  const double b = 0.01193*t_100_2 + 0.0851*t_100 + 1.02766;
  const double c = (1.8316*t_100_2 + -7.8119*t_100 + -3.6231) /
    (-0.10733*t_100_2 + 1.09192*t_100 + 1);

  const double log_p_pv = log(p.raw() - pv.raw());
  const double mch4pw = exp(a*log_p_pv*log_p_pv + b*log_p_pv + c);

  const double tk = Quantity<Kelvin>(t).raw();;
  const double p2 = p.raw()*p.raw();
  const double c1 = 7.015e-2 + 1.074e-4*tk + 2.260e-1*p.raw()/tk + -1.227e-3*p2/tk;
  constexpr double c2 = -6.28e-3;
  const double m = nacl_concentration.raw();
  const double m2 = m*m;

  const double mch4w = mch4pw * exp(-2*c1*m - c2*m2);

  return Quantity<Molality_NaCl>(mch4w);
}

inline Quantity<mPa_1>
CgfwSpiveyMN::impl(const Quantity<Celsius> & t,
		  const Quantity<mPascal> & p,
		  const Quantity<Molality_NaCl> & nacl_concentration) const
{
  const double m = nacl_concentration.raw(); 

  // Coefficients of compressibility of pure water
  const double t_100 = t.raw()/100;
  const double t_100_2 = t_100*t_100;

  const double epw = (4.221*t_100_2 + -3.478*t_100 + 6.221) / 
    (0.5182*t_100_2 + -0.4405*t_100 + 1);

  const double fpw = (-11.403*t_100_2 + 29.932*t_100 + 27.952) /
    (0.20684*t_100_2 + 0.3768*t_100 + 1);

  // Coefficients of gas-free brine compressibility
  constexpr double e =  0.1353;

  const double f1 = (-1.409*t_100_2 + -0.361*t_100 + -0.2532)/(9.216*t_100 + 1);
  const double f2 = (5.614*t_100 + 4.6782) / (-0.307*t_100_2 + 2.6069*t_100 + 1);
  const double f3 = -0.1127*t_100_2 + 0.2047*t_100 + -0.0452;
        
  const double ew = epw + e*m;
  const double fw = fpw + f1*pow(m, 1.5) + f2*m + f3*sqrt(m);

  // Compressibility of gas-free brine
  constexpr double inv_pref = 1.0/70.0; //Pressure of reference: 70 [MPa]
  const double p_70 = p.raw()/70;
  const double cgfw1 = 1.0/(ew*p_70 + fw);
  const double cgfw = inv_pref * cgfw1;

  return Quantity<mPa_1>(cgfw);
}

#endif // SPIVEY_DRY_IMPL_H