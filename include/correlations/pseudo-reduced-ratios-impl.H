# ifndef PSEUDO_REDUCED_RATIOS_IMPL_H
# define PSEUDO_REDUCED_RATIOS_IMPL_H

# include <correlations/pseudo-reduced-ratios.H>

inline double
Tpr::impl(const double & t,
	  const double & tpc) noexcept
{
  return t/tpc;
}

inline double
Ppr::impl(const double & p,
	  const double & ppc) noexcept
{
  return p/ppc;
}

# endif // PSEUDO_REDUCED_RATIOS_IMPL_H
