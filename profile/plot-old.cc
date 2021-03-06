
# include <memory>

# include <tclap/CmdLine.h>

# include <ah-zip.H>
# include <ah-dispatcher.H>
# include <tpl_dynMapTree.H>

# include <json.hpp>

using json = nlohmann::json;

# include <correlations/pvt-correlations.H>
# include <correlations/defined-correlation.H>

using namespace std;
using namespace TCLAP;

void error_msg(const string & msg)
{
  cout << msg << endl;
  abort();
}

CmdLine cmd = { "plot-corr", ' ', "0" };

// indica que se deben verificar rangos de parámetros de correlaciones
SwitchArg check_arg = { "", "check", "check correlation ranges", cmd };
bool check = false;
void set_check()
{
  check = check_arg.getValue();
}

// indica que las excepciones deben reportarse
SwitchArg catch_exceptions = { "", "exceptions", "report exceptions", cmd };
bool report_exceptions = false;
DynList<string> exception_list; // en esta lista se guaradan las excepciones

// nombres de parámetros permitidos (son valores o rangos, pero no son
// correlaciones). Esta tabla es usada para validad cambio de unidades
DynSetTree<string> par_name_tbl =
  { "api", "rsb", "yg", "tsep", "t", "p", "psep", "h2s-concentration",
    "co2-concentration", "n2-concentration", "nacl-concentration" };
struct ArgUnit
{
  string par_name;
  string unit_name;

  ArgUnit & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> par_name >> unit_name))
      throw TCLAP::ArgParseException(str + " is not a pair par-name unit");

    if (not par_name_tbl.contains(par_name))
      throw TCLAP::ArgParseException(par_name + " is an invalid parameter name");

    return *this;
  }

  ArgUnit() {}

  friend ostream& operator << (ostream &os, const ArgUnit & a) 
  {
    return os << a.par_name << " " << a.unit_name;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<ArgUnit> { typedef StringLike ValueCategory; };
}

// Especificación de cambio de unidad. Sirve para cualquier parámetro
MultiArg<ArgUnit> unit = { "", "unit", "unit \"par-name unit\"", false,
			   "unit \"par-name unit\"", cmd };

// Verifica si el parámetro par_name tiene un cambio de
// unidad. ref_unit es la unidad por omisión del parámetro. Si no hubo
// especificación de cambio de unidad para par_name, entonces se
// retorna ref_unit
const Unit * test_unit_change(const string & par_name, const Unit & ref_unit)
{
  if (not par_name_tbl.contains(par_name))
    {
      cout << "for option --unit " << par_name << ": invalid parameter name"
	   << endl;
      abort();
    }
  
  const Unit * ret = &ref_unit;
  for (const auto & par : unit.getValue()) // recorra lista de cambios
    if (par.par_name == par_name)
      {
	const Unit * ret = Unit::search_by_name(par.unit_name);
	if (ret == nullptr)
	  {
	    cout << "In unit change for " << par_name << ": unit name "
		 << par.unit_name << " not found" << endl;
	    abort();
	  }

	if (&ref_unit.physical_quantity != &ret->physical_quantity)
	  {
	    cout << "For " << par_name << " unit: physical quantity "
		 << ret->physical_quantity.name << " is invalid" << endl;
	    abort();
	  }
	return ret;
      }
  
  return ret;
}


// helper para manejar pase de parámetros a las correlaciones

// construye un VtlQuantity a partir de un parámetro por nombre
inline VtlQuantity par(const Correlation::NamedPar & par)
{
  return VtlQuantity(*get<3>(par), get<2>(par));
}

// construye un parámetro por nombre name a partir de una cantidad
// (VtlQuantity o Quantity<Unit>)
inline Correlation::NamedPar npar(const string & name, const BaseQuantity & p)
{
  return Correlation::NamedPar(true, name, p.raw(), &p.unit);
}

inline Correlation::NamedPar npar(const string & name, double v, const Unit * unit)
{
  return Correlation::NamedPar(true, name, v, unit);
}

inline Correlation::NamedPar npar(const string & name,
				  const Correlation::NamedPar & par)
{
  return Correlation::NamedPar(true, name, get<2>(par), get<3>(par));
}

// macro que construye un parámetro por nombre con nombre par a partir
// de una cantidad par
# define NPAR(par) npar(#par, par)

// Argumentos obrlogatorios en la línea de comandos que son validados
// en el parsing

ValueArg<double> api_arg = { "", "api", "api", true, 0, "api", cmd };
Correlation::NamedPar api_par;
void set_api()
{
  auto api_unit = test_unit_change("api", Api::get_instance());
  api_par = npar("api", api_arg.getValue(), api_unit);
}

ValueArg<double> rsb_arg = { "", "rsb", "rsb", true, 0, "rsb in scf/STB", cmd };
Correlation::NamedPar rsb_par;
const Unit * rsb_unit = nullptr;
void set_rsb()
{
  rsb_unit = test_unit_change("rsb", SCF_STB::get_instance());
  rsb_par = npar("rsb", rsb_arg.getValue(), rsb_unit);
}

ValueArg<double> yg_arg = { "", "yg", "yg", true, 0, "yg in Sgg", cmd };
Correlation::NamedPar yg_par;
VtlQuantity yg;
const Unit * yg_unit = nullptr;
void set_yg()
{
  yg_unit = test_unit_change("yg", Sgg::get_instance());
  yg_par = npar("yg", yg_arg.getValue(), yg_unit);
  yg.set(par(yg_par));
}

// optional command arguments

ValueArg<double> tsep_arg = { "", "tsep", "separator temperature", false, 0,
			      "separator temperature in degF", cmd };
Correlation::NamedPar tsep_par;
VtlQuantity tsep;
const Unit * tsep_unit = nullptr;
void set_tsep()
{
  tsep_unit = test_unit_change("tsep", Fahrenheit::get_instance());
  tsep_par = make_tuple(tsep_arg.isSet(), "tsep",
			tsep_arg.getValue(), tsep_unit);
  tsep.set(par(tsep_par));
}

ValueArg<double> psep_arg = { "", "psep", "separator pressure", false, 0,
			      "separator pressure in psia", cmd };
Correlation::NamedPar psep_par;
VtlQuantity psep;
const Unit * psep_unit = nullptr;
void set_psep()
{
  psep_unit = test_unit_change("psep", psia::get_instance());
  psep_par = make_tuple(psep_arg.isSet(), "psep",
			psep_arg.getValue(), psep_unit);
  psep.set(par(psep_par));
}

ValueArg<double> n2_concentration_arg =
  { "", "n2-concentration", "n2-concentration", false, 0,
    "n2-concentration in MolePercent", cmd };
Correlation::NamedPar n2_concentration_par;
VtlQuantity n2_concentration;
const Unit * n2_concentration_unit = nullptr;
void set_n2_concentration()
{
  n2_concentration_unit = test_unit_change("n2-concentration",
						MolePercent::get_instance());
  n2_concentration_par = make_tuple(n2_concentration_arg.isSet(),
				    "n2_concentration",
				    n2_concentration_arg.getValue(),
				    n2_concentration_unit);
  n2_concentration.set(par(n2_concentration_par));
}

ValueArg<double> co2_concentration_arg =
  { "", "co2-concentration", "co2-concentration", false, 0,
    "co2-concentration in MolePercent", cmd };
Correlation::NamedPar co2_concentration_par;
VtlQuantity co2_concentration;
const Unit * co2_concentration_unit = nullptr;
void set_co2_concentration()
{
  co2_concentration_unit = test_unit_change("co2-concentration",
					    MolePercent::get_instance());
  co2_concentration_par = make_tuple(co2_concentration_arg.isSet(),
				     "co2_concentration",
				     co2_concentration_arg.getValue(),
				     co2_concentration_unit);
  co2_concentration.set(par(co2_concentration_par));
}

ValueArg<double> h2s_concentration_arg =
  { "", "h2s-concentration", "h2s-concentration", false, 0,
    "h2s-concentration in MolePercent", cmd };
Correlation::NamedPar h2s_concentration_par;
VtlQuantity h2s_concentration;
const Unit * h2s_concentration_unit = nullptr;
void set_h2s_concentration()
{
  h2s_concentration_unit = test_unit_change("h2s-concentration",
					    MolePercent::get_instance());
  h2s_concentration_par = make_tuple(h2s_concentration_arg.isSet(),
				     "h2s_concentration",
				     h2s_concentration_arg.getValue(),
				     h2s_concentration_unit);
  h2s_concentration.set(par(h2s_concentration_par));
}

ValueArg<double> nacl_concentration_arg =
  { "", "nacl-concentration", "nacl-concentration", false, 0,
    "nacl-concentration in mol_NaCl/Kg_H2O", cmd };
Correlation::NamedPar nacl_concentration_par;
VtlQuantity nacl_concentration;
const Unit * nacl_concentration_unit = nullptr;
void set_nacl_concentration()
{
  nacl_concentration_unit = test_unit_change("nacl-concentration",
					     Molality_NaCl::get_instance());
  nacl_concentration_par = make_tuple(nacl_concentration_arg.isSet(),
				      "nacl_concentration",
				      nacl_concentration_arg.getValue(),
				      nacl_concentration_unit);
  nacl_concentration.set(par(nacl_concentration_par));
}

ValueArg<double> c_pb_arg = { "", "c-pb", "pb adjustment", false, 0,
			      "pb adjustment", cmd };
ValueArg<string> pb_arg = { "", "pb", "correlation for pb", true, "",
			    "correlation for pb", cmd };
const Correlation * pb_corr = nullptr;
ParList pb_pars;
void set_pb()
{
  assert(yg_arg.isSet() and rsb_arg.isSet() and api_arg.isSet());
  pb_corr = Correlation::search_by_name(pb_arg.getValue());
  if (pb_corr == nullptr)
    error_msg(pb_arg.getValue() + " correlation not found");

  if (pb_corr->target_name() != "pb")
    error_msg(pb_arg.getValue() + " correlation is not for pb");

  set_api();
  set_rsb();
  set_yg();
  set_tsep();
  set_psep();
  set_h2s_concentration();
  set_co2_concentration();
  set_n2_concentration();
  set_nacl_concentration();

  pb_pars.insert(yg_par);
  pb_pars.insert(rsb_par);
  pb_pars.insert(api_par);
}

VtlQuantity compute_pb(const double t)
{
  pb_pars.insert("t", t, &Fahrenheit::get_instance());
  auto ret = pb_corr->tuned_compute_by_names(pb_pars,
					     c_pb_arg.getValue(), 0, check);
  pb_pars.remove("t");
  return ret;
}

// Inicializa una correlación especificada desde la línea de comandos
// vía corr_name_arg. Verifica que la correlación sea para la
// propiedad target_name y si es el caso entonces la correlación
// encontrada se coloca en el parámetro de salida corr_ptr.
//
// El parámetro force_set indica que es obligatorio que la correlación
// sea especificada en la línea de comandos.
void set_correlation(ValueArg<string> & corr_name_arg,
		     const string & target_name,
		     const Correlation *& corr_ptr,
		     bool force_set = false)
{
  if (force_set and not corr_name_arg.isSet())
    error_msg("Correlation for "+ target_name +
	      " has not been specified in command line (" +
	      corr_name_arg.longID() + ")");
  if (not corr_name_arg.isSet())
    return;
  const string & corr_name = corr_name_arg.getValue();
  corr_ptr = Correlation::search_by_name(corr_name);
  if (corr_ptr == nullptr)
    error_msg("Correlation for " + target_name + " " +
	      corr_name +  " not found");
  if (corr_ptr->target_name() != target_name)
    error_msg("Correlation " + corr_ptr->name + " is not for " + target_name);
}

ValueArg<double> c_rs_arg = { "", "c-rs", "rs c", false, 0, "rs c", cmd };
ValueArg<double> m_rs_arg = { "", "m-rs", "rs m", false, 1, "rs m", cmd };
ValueArg<string> rs_corr_arg = { "", "rs", "correlation for rs", false, "",
				 "correlation for rs", cmd };
const Correlation * rs_corr = nullptr;
void set_rs_corr(bool force = false)
{ set_correlation(rs_corr_arg, "rs", rs_corr, force); }

ValueArg<double> c_bob_arg = { "", "c-bob", "ob c", false, 0, "bob c", cmd };
ValueArg<double> m_bob_arg = { "", "m-bob", "bob m", false, 1, "bob m", cmd };
ValueArg<string> bob_corr_arg = { "", "bob", "correlation for bob", false, "",
				  "correlation for bob", cmd };
const Correlation * bob_corr = nullptr;
void set_bob_corr(bool force = false)
{ set_correlation(bob_corr_arg, "bob", bob_corr, force); }

ValueArg<double> c_boa_arg = { "", "c-boa", "ob c", false, 0, "boa c", cmd };
ValueArg<double> m_boa_arg = { "", "m-boa", "boa m", false, 1, "boa m", cmd };
ValueArg<string> boa_corr_arg = { "", "boa", "correlation for boa", false, "",
				  "correlation for boa", cmd };
const Correlation * boa_corr = nullptr;
void set_boa_corr(bool force = false)
{ set_correlation(boa_corr_arg, "boa", boa_corr, force); }

ValueArg<double> c_uod_arg = { "", "c-uod", "uod c", false, 0, "uod c", cmd };
ValueArg<double> m_uod_arg = { "", "m-ruod", "uod m", false, 1, "uod m", cmd };
ValueArg<string> uod_corr_arg = { "", "uod", "correlation for uod", false, "",
				 "correlation for ruod", cmd };
const Correlation * uod_corr = nullptr;
void set_uod_corr(bool force = false)
{ set_correlation(uod_corr_arg, "uod", uod_corr, force); }

ValueArg<double> c_cob_arg = { "", "c-cob", "cob c", false, 0, "cob c", cmd };
ValueArg<double> m_cob_arg = { "", "m-cob", "cob m", false, 1, "cob m", cmd };
ValueArg<string> cob_corr_arg = { "", "cob", "correlation for cob", false, "",
				  "correlation for cob", cmd };
const Correlation * cob_corr = nullptr;
void set_cob_corr(bool force = false)
{ set_correlation(cob_corr_arg, "cob", cob_corr, force); }

ValueArg<double> c_coa_arg = { "", "c-coa", "coa c", false, 0, "coa c", cmd };
ValueArg<double> m_coa_arg = { "", "m-coa", "coa m", false, 1, "coa m", cmd };
ValueArg<string> coa_corr_arg = { "", "coa", "correlation for coa", false, "",
				  "correlation for coa", cmd };
const Correlation * coa_corr = nullptr;
void set_coa_corr(bool force = false)
{ set_correlation(coa_corr_arg, "coa", coa_corr, force); }

ValueArg<double> c_uob_arg = { "", "c-uob", "uob c", false, 0, "uob c", cmd };
ValueArg<double> m_uob_arg = { "", "m-uob", "uob m", false, 1, "uob m", cmd };
ValueArg<string> uob_corr_arg = { "", "uob", "correlation for uob", false, "",
				  "correlation for uob", cmd };
const Correlation * uob_corr = nullptr;
void set_uob_corr(bool force = false)
{ set_correlation(uob_corr_arg, "uob", uob_corr, force); }

ValueArg<double> c_uoa_arg = { "", "c-uoa", "uoa c", false, 0, "uoa c", cmd };
ValueArg<double> m_uoa_arg = { "", "m-uoa", "uoa m", false, 1, "uoa m", cmd };
ValueArg<string> uoa_corr_arg = { "", "uoa", "correlation for uoa", false, "",
				  "correlation for uoa", cmd };
const Correlation * uoa_corr = nullptr;
void set_uoa_corr(bool force = false)
{ set_correlation(uoa_corr_arg, "uoa", uoa_corr, force); }

ValueArg<string> ppchc_corr_arg = { "", "ppchc", "Correlation for ppchc",
				    false, "", "Correlation for ppchc", cmd };
const Correlation * ppchc_corr = nullptr;
void set_ppchc_corr()
{
  ppchc_corr = &PpchcStanding::get_instance(); // mandotory here!
  set_correlation(ppchc_corr_arg, "ppchc", ppchc_corr, false);
}

ValueArg<string> ppcm_mixing_corr_arg =
  { "", "ppcm-mixing", "Correlation for ppcm mixing rule",
    false, "PpcmKayMixingRule", "Correlation for ppcm mixing rule", cmd };
const Correlation * ppcm_mixing_corr = nullptr;
void set_ppcm_mixing_corr()
{
  ppcm_mixing_corr = &PpcmKayMixingRule::get_instance(); // mandotory here!
  set_correlation(ppcm_mixing_corr_arg, "ppccm", ppcm_mixing_corr, false);
}

ValueArg<string> adjustedppcm_corr_arg =
  { "", "adjustedppcm", "Correlation for ajustedppcm",
    false, "AdjustedppcmWichertAziz", "Correlation for adjustedppcm", cmd };
const Correlation * adjustedppcm_corr = nullptr;
void set_adjustedppcm_corr()
{
  adjustedppcm_corr = &AdjustedppcmWichertAziz::get_instance(); // mandotory here!
  set_correlation(adjustedppcm_corr_arg, "adjustedppccm",
		  adjustedppcm_corr, false);
}

ValueArg<string> tpchc_corr_arg = { "", "tpchc", "Correlation for tpchc",
				    false, "", "Correlation for tpchc", cmd };
const Correlation * tpchc_corr = nullptr;
void set_tpchc_corr()
{
  tpchc_corr = &TpchcStanding::get_instance(); // by default
  set_correlation(tpchc_corr_arg, "tpchc", tpchc_corr, false);
}

ValueArg<string> tpcm_mixing_corr_arg =
  { "", "tpcm", "Correlation for tpcm mixing rule",
    false, "TpcmKayMixingRule", "Correlation for tpcm mixing rule", cmd };
const Correlation * tpcm_mixing_corr = nullptr;
void set_tpcm_mixing_corr()
{
  tpcm_mixing_corr = &TpcmKayMixingRule::get_instance(); // mandotory here!
  set_correlation(tpcm_mixing_corr_arg, "tpcm", tpcm_mixing_corr, false);
}

ValueArg<string> adjustedtpcm_corr_arg =
  { "", "adjustedtpcm", "Correlation for adjustedtpcm",
    false, "AdjustedtpcmWichertAziz", "Correlation for adjustedtpcm", cmd };
const Correlation * adjustedtpcm_corr = nullptr;
void set_adjustedtpcm_corr()
{
  adjustedtpcm_corr = &AdjustedtpcmWichertAziz::get_instance(); // mandotory here!
  set_correlation(adjustedtpcm_corr_arg, "adjustedtpcm", adjustedtpcm_corr, false);
}

ValueArg<string> zfactor_corr_arg =
  { "", "zfactor", "Correlation for zfactor", false, "",
    "Correlation for zfactor", cmd };
const Correlation * zfactor_corr = nullptr;
void set_zfactor_corr()
{
  zfactor_corr = &ZfactorDranchukAK::get_instance(); // by default
  set_correlation(zfactor_corr_arg, "zfactor", zfactor_corr, false);
}

ValueArg<string> cg_corr_arg =
  { "", "cg", "Correlation for cg", false, "", "Correlation for cg", cmd };
const Correlation * cg_corr = nullptr;
void set_cg_corr()
{
  cg_corr = &CgMattarBA::get_instance();
  set_correlation(cg_corr_arg, "cg", cg_corr, false);
}

ValueArg<string> ug_corr_arg =
  { "", "ug", "Correlation for ug", false, "", "Correlation for ug", cmd };
const Correlation * ug_corr = nullptr;
void set_ug_corr()
{
  ug_corr = &UgCarrKB::get_instance(); // by default
  set_correlation(ug_corr_arg, "ug", ug_corr, false);
}

ValueArg<string> bwb_corr_arg =
  { "", "bwb", "Correlation for bwb", false, "", "Correlation for bwb", cmd };
const Correlation * bwb_corr = nullptr;
void set_bwb_corr()
{
  bwb_corr = &BwbSpiveyMN::get_instance();
  set_correlation(bwb_corr_arg, "bwb", bwb_corr, false);
}

ValueArg<string> bwa_corr_arg =
  { "", "bwa", "Correlation for bwa", false, "", "Correlation for bwa", cmd };
const Correlation * bwa_corr = nullptr;
void set_bwa_corr()
{
  bwa_corr = &BwaSpiveyMN::get_instance();
  set_correlation(bwa_corr_arg, "bwa", bwa_corr, false);
}

ValueArg<string> uw_corr_arg =
  { "", "uw", "Correlation for uw", false, "", "Correlation for uw", cmd };
const Correlation * uw_corr = nullptr;
void set_uw_corr()
{
  uw_corr = &UwMcCain::get_instance();
  set_correlation(uw_corr_arg, "uw", uw_corr, false);
}

ValueArg<string> pw_corr_arg =
  { "", "pw", "Correlation for pw", false, "", "Correlation for pw", cmd };
const Correlation * pw_corr = nullptr;
void set_pw_corr()
{
  pw_corr = &PwSpiveyMN::get_instance();
  set_correlation(pw_corr_arg, "pw", pw_corr, false);
}

ValueArg<string> cwb_corr_arg =
  { "", "cwb", "Correlation for cwb", false, "", "Correlation for cwb", cmd };
const Correlation * cwb_corr = nullptr;
void set_cwb_corr()
{
  cwb_corr = &CwbSpiveyMN::get_instance();
  set_correlation(cwb_corr_arg, "cwb", cwb_corr, false);
}

ValueArg<string> cwa_corr_arg =
  { "", "cwa", "Correlation for cwa", false, "", "Correlation for cwa", cmd };
const Correlation * cwa_corr = nullptr;
void set_cwa_corr()
{
  cwa_corr = &CwaSpiveyMN::get_instance();
  set_correlation(cwa_corr_arg, "cwa", cwa_corr, false);
}

ValueArg<string> rsw_corr_arg =
  { "", "rsw", "Correlation for rsw", false, "", "Correlation for rsw", cmd };
const Correlation * rsw_corr = nullptr;
void set_rsw_corr()
{
  rsw_corr = &RswSpiveyMN::get_instance();
  set_correlation(rsw_corr_arg, "rsw", rsw_corr, false);
}

ValueArg<string> sgo_corr_arg =
  { "", "sgo", "Correlation for sgo", false, "", "Correlation for sgo", cmd };
const Correlation * sgo_corr = nullptr;
void set_sgo_corr()
{
  set_correlation(sgo_corr_arg, "sgo", sgo_corr, true);
}

ValueArg<string> sgw_corr_arg =
  { "", "sgw", "Correlation for sgw", false, "", "Correlation for sgw", cmd };
const Correlation * sgw_corr = nullptr;
void set_sgw_corr()
{
  sgw_corr = &SgwJenningsNewman::get_instance();
  set_correlation(sgw_corr_arg, "sgw", sgw_corr, false);
}


vector<string> grid_types =
  { "blackoil", "wetgas", "drygas", "brine", "gascondensated" };
ValuesConstraint<string> allowed_grid_types = grid_types;
ValueArg<string> grid = { "", "grid", "grid type", false,
			  "blackoil", &allowed_grid_types, cmd };

SwitchArg print_types = { "", "fluid-types", "print fluid types", cmd };

void print_fluid_types()
{
  assert(print_types.isSet() and print_types.getValue());

  for (auto & type : grid_types)
    cout << type << endl;
  exit(0);
}

struct RangeDesc
{
  double min = 0, max = 0;
  size_t n = 1; // num of steps

  RangeDesc & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> min >> max >> n))
      throw TCLAP::ArgParseException(str + " is not of form \"min max n\"");

    if (min > max)
      {
	ostringstream s;
	s << "min value " << min <<  " greater than max value " << max;
	throw TCLAP::ArgParseException(s.str());
      }

    return *this;
  }

  double step() const noexcept { return (max - min) / (n - 1); }

  friend ostream & operator << (ostream & os, const RangeDesc & d)
  {
    return os << d.min<< " " << d.max << " " << d.n;
  }
  
  ostream& print(ostream &os) const
  {
    return os << *this;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<RangeDesc> { typedef StringLike ValueCategory; };
}

void set_range(const RangeDesc & range, const string & name,
	       const Unit & unit, DynList<Correlation::NamedPar> & l)
{
  assert(l.is_empty());
  const auto & step = range.step();
  double val = range.min;
  for (size_t i = 0; i < range.n; ++i, val += step)
    l.append(make_tuple(true, name, val, &unit));
}

ValueArg<RangeDesc> t_range =
  { "", "t", "min max n", true, RangeDesc(),
    "range spec \"min max n\" for temperature", cmd };
DynList<Correlation::NamedPar> t_values;
const Unit * t_unit = nullptr;
void set_t_range()
{
  t_unit = test_unit_change("t", Fahrenheit::get_instance());
  set_range(t_range.getValue(), "t", *t_unit, t_values);
}

ValueArg<RangeDesc> p_range =
  { "", "p", "min max n", true, RangeDesc(),
    "range spec \"min max n\" for pressure", cmd };
DynList<Correlation::NamedPar> p_values;
const Unit * p_unit = nullptr;
void set_p_range()
{
  p_unit = test_unit_change("p", psia::get_instance());
  set_range(p_range.getValue(), "p", *p_unit, p_values);
}

ValueArg<double> cb_arg = { "", "cb", "c for below range", false, 0,
			    "c for below range", cmd };

ValueArg<double> mb_arg = { "", "mb", "m for below range", false, 1,
			    "m for below range", cmd };

ValueArg<double> ca_arg = { "", "ca", "m for above range", false, 0,
			    "c for above range", cmd };

ValueArg<double> ma_arg = { "", "ma", "m for above range", false, 1,
			    "c for above range", cmd };

string target_name;

ValueArg<string> below_corr_arg = { "", "below", "below correlation name", false, 
				    "", "below correlation name", cmd };
const Correlation * below_corr_ptr = nullptr;
void set_below_corr()
{
  if (grid.isSet())
    error_msg("grid option is incompatible with below option");
  below_corr_ptr = Correlation::search_by_name(below_corr_arg.getValue());
  if (below_corr_ptr == nullptr)
    error_msg(below_corr_arg.getValue() + " below correlation not found");
  target_name = below_corr_ptr->target_name();
  if (target_name != "rs" and target_name != "bob" and target_name != "uob")
    error_msg(below_corr_arg.getValue() +
	      " below correlation has an invalid target name"
	      " (must be rs, bob or uob family)");
  if ((target_name == "bob" or target_name == "uob") and not rs_corr_arg.isSet())
    error_msg("Correlation for rs has not been specified");
  if (target_name == "uob" and not bob_corr_arg.isSet())
    error_msg("Correlation for bob has not been specified");
  if (target_name == "uob" and not boa_corr_arg.isSet())
    error_msg("Correlation for boa has not been specified");
  if (target_name == "uob" and not uod_corr_arg.isSet())
    error_msg("Correlation for uod has not been specified");
}

ValueArg<string> above_corr_arg = { "", "above", "above correlation name", false,
				    "", "above correlation name", cmd };
const Correlation * above_corr_ptr = nullptr;
void set_above_corr()
{
  if (grid.isSet())
    error_msg("grid option is incompatible with above option");
  if (target_name == "rs")
    {
      if (not above_corr_arg.isSet())
	{
	  above_corr_ptr = &RsAbovePb::get_instance();
	  return;
	}
      else
	error_msg("Above correlation must not be defined for rs");
    }

  above_corr_ptr = Correlation::search_by_name(above_corr_arg.getValue());
  if (above_corr_ptr == nullptr)
    error_msg(above_corr_arg.getValue() + " above correlation not found");
  
  const string above_target_name = above_corr_ptr->target_name();
  if (above_target_name == "boa")
    {
      if (target_name != "bob")
	error_msg("Above target " + above_target_name +
		  " cannot be combined with below correlation " +
		  below_corr_ptr->name);
      else
	return;
    }

  if (above_target_name == "uoa" and target_name == "uob")
    return;
  
  error_msg("Above target " + above_target_name + " cannot be combined with "
	    "below correlation " + below_corr_ptr->name);
}

vector<string> out_type = { "csv", "mat", "json", "R" };
ValuesConstraint<string> allowed_out_type = out_type;
ValueArg<string> output_type = { "t", "output-type", "output type", false,
				 "mat", &allowed_out_type, cmd };

enum class OutputType { mat, csv, R, json, undefined };

OutputType get_output_type(const string & type)
{
  if (type == "mat") return OutputType::mat;
  if (type == "csv") return OutputType::csv;
  if (type == "R") return OutputType::R;
  if (type == "json") return OutputType::json;
  error_msg("Invalid output type " + type);
  return OutputType::undefined;
}

vector<string> sort_values = { "p", "t", "value"};
ValuesConstraint<string> allow_sort = sort_values;
ValueArg<string> sort_type = { "s", "sort", "output sorting type", false,
			       "t", &allow_sort, cmd };

enum class SortType { Pressure, Temperature, Value, Undefined };

SortType get_sort_type(const string & type)
{
  if (type == "p") return SortType::Pressure;
  if (type == "t") return SortType::Temperature;
  if (type == "value") return SortType::Value;
  error_msg("Invalid sort type");
  return SortType::Undefined;
}

DefinedCorrelation
define_correlation(const VtlQuantity & pb,
		   const Correlation * below_corr_ptr, double cb, double mb,
		   const Correlation * above_corr_ptr,
		   double ca = 0, double ma = 1)
{
  DefinedCorrelation ret("p", pb.unit);
  ret.add_tuned_correlation(below_corr_ptr, psia::get_instance().min(), pb,
			    cb, mb);
  ret.add_tuned_correlation(above_corr_ptr, pb.next(),
			    psia::get_instance().max(), ca, ma);
  return ret;
}

DefinedCorrelation
define_correlation(const VtlQuantity & pb,
		   const Correlation * below_corr_ptr, 
		   const Correlation * above_corr_ptr)
{
  DefinedCorrelation ret("p", pb.unit);
  ret.add_tuned_correlation(below_corr_ptr, psia::get_instance().min(), pb,
			    0, 1);
  ret.add_tuned_correlation(above_corr_ptr, pb.next(),
			    psia::get_instance().max(), 0, 1);
  return ret;
}

DefinedCorrelation target_correlation(const VtlQuantity & pb)
{
  return define_correlation(pb, below_corr_ptr,
			    cb_arg.getValue(), mb_arg.getValue(),
			    above_corr_ptr, 
			    ca_arg.getValue(), ma_arg.getValue());
}

DefinedCorrelation
set_rs_correlation(const VtlQuantity & pb,
		   const VtlQuantity & rsb, const Correlation * rs_corr,
		   double c, double m)
{
  DefinedCorrelation ret = define_correlation(pb, rs_corr, c, m,
					      &RsAbovePb::get_instance());
  ret.set_max(rsb);
  return ret;
}

void
test_parameter(const DynList<pair<string, DynList<string>>> & required,
	       const Correlation::NamedPar & par, ParList & pars_list)
{
  if (required.exists([&par] (const auto & p) { return p.first == get<1>(par) or
      p.second.exists([&par] (const auto & s) { return s == get<1>(par); }); }))
    pars_list.insert(par);
}

ParList compute_pb_and_load_constant_parameters()
{
  ParList pars_list;
  auto t_par = t_values.get_first();
  pb_pars.insert(t_par);
  auto pb =
    pb_corr->tuned_compute_by_names(pb_pars, c_pb_arg.getValue(), 1, check);
  pb_pars.remove(t_par);
  auto required_pars = target_correlation(pb).parameter_list();
  test_parameter(required_pars, api_par, pars_list);
  test_parameter(required_pars, rsb_par, pars_list);
  test_parameter(required_pars, yg_par, pars_list);
  test_parameter(required_pars, tsep_par, pars_list);
  test_parameter(required_pars, psep_par, pars_list);
  test_parameter(required_pars, n2_concentration_par, pars_list);
  test_parameter(required_pars, co2_concentration_par, pars_list);
  test_parameter(required_pars, h2s_concentration_par, pars_list);

  return pars_list;
}

const double Invalid_Value = Unit::Invalid_Value;

double temperature = 0, pressure = 0; // valores globales y puestos
				      // por el grid mientras itera 

bool exception_thrown = false;

void store_exception(const string & corr_name, const exception & e)
{
  exception_thrown = true;
  ostringstream s;
  s << corr_name << ": " << VtlQuantity(*t_unit, temperature)
    << ", " << VtlQuantity(*p_unit, pressure) << ": " << e.what()
    << endl;
  exception_list.append(s.str());
}

/* meta inserta par en pars_list pero se detiene si algún parámetro es
  inválido. 

  Retorna true si todos los parámetros fueron válidos (!= Invalid_Value)

  De lo contrario la inserción se detiene en el primer parámetro
  inválido, los parámetros previamente insertados en la lista son
  eliminados y se retorna false
*/
inline bool insert_in_pars_list(ParList&) { return true; }

  template <typename ... Args> inline
bool insert_in_pars_list(ParList & pars_list, 
			 const Correlation::NamedPar & par, Args & ... args)
{
  if (get<2>(par) == Invalid_Value)
    return false;
  
  pars_list.insert(par);
  if (insert_in_pars_list(pars_list, args...))
    return true;

  pars_list.remove(par); // si inserción recursiva falla ==> eliminar par

  return false;
}

template <typename ... Args> inline
VtlQuantity compute(const Correlation * corr_ptr,
		    double c, double m, bool check,
		    ParList & pars_list, Args ... args)
{
  try
    {
      if (not insert_in_pars_list(pars_list, args...))
	return VtlQuantity();

      auto ret = corr_ptr->tuned_compute_by_names(pars_list, c, m, check);
      remove_from_container(pars_list, args...);
      return ret;
    }
  catch (exception & e)
    {
      if (report_exceptions)
	store_exception(corr_ptr->name, e);
      
      remove_from_container(pars_list, args ...);
    }
  return VtlQuantity();
}

template <typename ... Args> inline
VtlQuantity dcompute(const Correlation * corr_ptr, bool check,
		    ParList & pars_list, Args ... args)
{
  try
    {
      if (not insert_in_pars_list(pars_list, args...))
	return VtlQuantity();

      auto ret = corr_ptr->compute_by_names(pars_list, check);
      remove_from_container(pars_list, args...);
      return ret;
    }
  catch (exception & e)
    {
      if (report_exceptions)
	store_exception(corr_ptr->name, e);
      
      remove_from_container(pars_list, args ...);
    }
  return VtlQuantity();
}

template <typename ... Args> inline
string correlation_call(const Correlation * corr_ptr, Args ... args)
{
  DynList<Correlation::NamedPar> pars;
  append_in_container(pars, args...);

  ostringstream s;
  s << corr_ptr->name << "(";
  for (auto it = pars.get_it(); it.has_curr(); it.next())
    {
      const auto & par = it.get_curr();
      s << get<1>(par) << " = " << get<2>(par) << " " << get<3>(par)->name;
      if (&par != &pars.get_last())
	s << ", ";
    }
  s << ")";
  return s.str();
}

template <typename ... Args> inline
VtlQuantity dcompute_noexcep(const Correlation * corr_ptr, bool check,
			     ParList & pars_list, Args ... args)
{
  try
    {
      if (not insert_in_pars_list(pars_list, args...))
	return VtlQuantity();

      auto ret = corr_ptr->compute_by_names(pars_list, check);
      remove_from_container(pars_list, args...);
      return ret;
    }
  catch (exception & e)
    {
      remove_from_container(pars_list, args ...);
      cout << "ERROR initializing " << correlation_call(corr_ptr, args...)
	   << "@ " << e.what();
      abort();
    }
  return VtlQuantity();
}

// returna true si todos los args... son válidos
inline bool valid_args() { return true; }
  template <typename ... Args> inline
bool valid_args(const VtlQuantity & par, Args ... args)
{
  if (par.is_null())
    return false;
  return valid_args(args...);
}  

// Macro para crear variable var con el valor de la correlación corr_name
# define CALL(corr_name, var, args...)					\
  VtlQuantity var;							\
  try									\
    {									\
      if (valid_args(args))						\
	var = corr_name::get_instance().call(args);			\
    }									\
  catch (exception & e)							\
    {									\
      store_exception(corr_name::get_instance().name, e);		\
    }

inline bool
insert_in_pars_list(const DefinedCorrelation&, const VtlQuantity&, ParList&)
{
  return true;
}

  template <typename ... Args> inline
bool insert_in_pars_list(const DefinedCorrelation & corr,
			 const VtlQuantity & p_q,
			 ParList & pars_list,
			 const Correlation::NamedPar & par,
			 Args & ... args)
{
  if (get<2>(par) == Invalid_Value)
    {
      const string & par_name = get<1>(par);      
      if (corr.search_parameters(p_q).contains(par_name))
	return false; // aquí la correlación recibiría Invalid_Value y fallaría
    }
  
  pars_list.insert(par);
  if (insert_in_pars_list(corr, p_q, pars_list, args...))
    return true;

  pars_list.remove(par); // si inserción recursiva falla ==> eliminar par

  return false;
}

template <typename ... Args> inline
double compute(const DefinedCorrelation & corr, bool check,
	       const VtlQuantity & p_q,
	       ParList & pars_list, Args ... args)
{
  try
    {
      if (not insert_in_pars_list(corr, p_q, pars_list, args...))
	return Invalid_Value;
      double ret = corr.compute_by_names(pars_list, check);
      remove_from_container(pars_list, args ...);
      return ret;
    }
  catch (exception & e)
    {
      if (report_exceptions)
	{
	  auto triggering_corr_ptr = corr.search_correlation(p_q);
	  string names = corr.correlations().
	    foldl<string>("", [triggering_corr_ptr] (auto & acu, auto ptr)
            {
	      if (triggering_corr_ptr == ptr)
		return acu + "*" + ptr->name + " ";
	      return acu + ptr->name + " ";
	    });
	  store_exception("{ " + names + "}", e);
	}

      remove_from_container(pars_list, args ...);
    }
  return Invalid_Value;
}

template <typename ... Args> inline
VtlQuantity compute(const Correlation * corr_ptr,
		    double c, double m, bool check, Args ... args)
{ // se pone estática para crearla una sola vez. Pero cuidado! la
  // función deja de ser reentrante y no puede usarse en un entorno multithread
  static ParList pars_list;
  return compute(corr_ptr, c, m, check, pars_list, args...);
}

template <typename ... Args> inline
VtlQuantity dcompute(const Correlation * corr_ptr, bool check, Args ... args)
{ // se pone estática para crearla una sola vez. Pero cuidado! la
  // función deja de ser reentrante y no puede usarse en un entorno multithread
  static ParList pars_list;
  return dcompute(corr_ptr, check, pars_list, args...);
}

template <typename ... Args> inline
VtlQuantity
dcompute_noexcep(const Correlation * corr_ptr, bool check, Args ... args)
{ // se pone estática para crearla una sola vez. Pero cuidado! la
  // función deja de ser reentrante y no puede usarse en un entorno multithread
  static ParList pars_list;
  return dcompute_noexcep(corr_ptr, check, pars_list, args...);
}

template <typename ... Args> inline
double compute(const DefinedCorrelation & corr, bool check,
	       const VtlQuantity & p_q, Args ... args)
{ // se pone estática para crearla una sola vez. Pero cuidado! la
  // función deja de ser reentrante y no puede usarse en un entorno multithread
  static ParList pars_list;
  return compute(corr, check, pars_list, p_q, args...);
}

/// target, p, t
DynList<DynList<double>> generate_rs_values()
{
  auto pars_list = compute_pb_and_load_constant_parameters();

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();
      pars_list.insert(t_par);
      row.insert(get<2>(t_par));

      auto pb = compute(pb_corr, c_pb_arg.getValue(), 1, check, pb_pars, t_par);

      pars_list.insert(NPAR(pb));
      auto rs_corr =
	set_rs_correlation(pb, VtlQuantity(*rsb_unit, rsb_arg.getValue()),
			   below_corr_ptr, cb_arg.getValue(), mb_arg.getValue());

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();
	  VtlQuantity p_q = par(p_par);
	  auto val = compute(rs_corr, check, p_q, pars_list, p_par);
	  size_t n = row.ninsert(get<2>(p_par), val);
	  vals.append(row);
	  row.mutable_drop(n);
	}

      row.remove_first(); // t_par

      remove_from_container(pars_list, "pb", "t");
    }

  return vals;
}

/// Retorna la lista de parámetros necesarios por el conjunto de
/// correlaciones que están en l
ParList load_constant_parameters(const DynList<const Correlation*> & l)
{
  ParList pars_list;
  auto required_pars = DefinedCorrelation::parameter_list(l);
				    
  test_parameter(required_pars, api_par, pars_list);
  test_parameter(required_pars, rsb_par, pars_list);
  test_parameter(required_pars, yg_par, pars_list);
  test_parameter(required_pars, tsep_par, pars_list);
  test_parameter(required_pars, psep_par, pars_list);
  test_parameter(required_pars, n2_concentration_par, pars_list);
  test_parameter(required_pars, co2_concentration_par, pars_list);
  test_parameter(required_pars, h2s_concentration_par, pars_list);
  test_parameter(required_pars, nacl_concentration_par, pars_list);

  return pars_list;
}

/// target, p, t
DynList<DynList<double>> generate_bo_values()
{
  assert(rs_corr);
  auto bo_pars_list = compute_pb_and_load_constant_parameters();
  
  auto rs_pars_list =
    load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();

      row.insert(get<2>(t_par));

      auto pb =
	compute(pb_corr, c_pb_arg.getValue(), 1, check, pb_pars, t_par);
      auto pb_par = NPAR(pb);

      auto bo_corr = target_correlation(pb);
      insert_in_container(bo_pars_list, t_par, pb_par);

      auto rs_corr = define_correlation(pb, ::rs_corr,
					c_rs_arg.getValue(), m_rs_arg.getValue(),
					&RsAbovePb::get_instance());
      insert_in_container(rs_pars_list, t_par, pb_par);

      // último bo de la correlación bob. Este será el punto de partida de boa
      auto bobp =
	compute(below_corr_ptr, cb_arg.getValue(), mb_arg.getValue(),
		check, bo_pars_list, npar("p", pb), npar("rs", rsb_par));

      bo_pars_list.insert(NPAR(bobp));

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();
	  VtlQuantity p_q = par(p_par);
	  auto rs = compute(rs_corr, check, p_q, rs_pars_list, p_par);
	  auto bo = compute(bo_corr, check, p_q, bo_pars_list, p_par,
			    npar("rs", rs, &::rs_corr->unit));
	  size_t n = row.ninsert(get<2>(p_par), bo);
	  vals.append(row);
	  row.mutable_drop(n);
	}

      row.remove_first(); // t_par

      remove_from_container(rs_pars_list, pb_par, t_par); 
      remove_from_container(bo_pars_list, "bobp", "pb", t_par);
    }

  return vals;
}

/// target, p, t
DynList<DynList<double>> generate_uo_values()
{
  assert(rs_corr);
  auto uo_pars_list = compute_pb_and_load_constant_parameters();
  
  auto rs_pars_list =
    load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});

  auto uod_pars_list = load_constant_parameters({uod_corr});

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();
      row.insert(get<2>(t_par));

      auto pb = compute(pb_corr, c_pb_arg.getValue(), 1, check, pb_pars, t_par);
      auto pb_par = NPAR(pb);

      auto uod = dcompute(uod_corr, check, uod_pars_list, t_par);

      insert_in_container(rs_pars_list, t_par, pb_par);
      auto rs_corr = define_correlation(pb, ::rs_corr,
					c_rs_arg.getValue(), m_rs_arg.getValue(),
					&RsAbovePb::get_instance());

      auto uo_corr = target_correlation(pb);
      insert_in_container(uo_pars_list, t_par, pb_par, NPAR(uod));

      auto uobp =
	compute(below_corr_ptr, cb_arg.getValue(), mb_arg.getValue(),
		check, uo_pars_list, npar("p", pb), npar("rs", rsb_par));

      uo_pars_list.insert("uobp", uobp.raw(), &uobp.unit);

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();
	  auto p_q = par(p_par);
	  auto rs = compute(rs_corr, check, p_q, rs_pars_list, p_par);
	  auto uo = compute(uo_corr, check, p_q, uo_pars_list, p_par,
			    npar("rs", rs, &::rs_corr->unit));
	  size_t n = row.ninsert(get<2>(p_par), uo);
	  vals.append(row);
	  row.mutable_drop(n);
	}

      row.remove_first(); // t_par

      remove_from_container(rs_pars_list, "pb", t_par);
      remove_from_container(uo_pars_list, "uobp", "pb", "uod", t_par);
    }

  return vals;
}

void print_row(const FixedStack<double> & row, bool is_pb = false)
{
  const size_t n = row.size();
  double * ptr = &row.base();

  if (is_pb)
    printf("\"true\",");
  else
    printf("\"false\",");

  if (exception_thrown)
    {
      printf("\"true\",");
      exception_thrown = false;
    }
  else
    printf("\"false\",");

  for (long i = n - 1; i >= 0; --i)
    {
      const auto & val = ptr[i];
      if (val != Invalid_Value)
	printf("%f", val);
      //printf("%.17g", val);
      if (i > 0)
	printf(",");
    }
  printf("\n");
}

template <typename ... Args>
void print_csv_header(Args ... args)
{
  FixedStack<pair<string, const Unit*>> header;
  insert_in_container(header, args...);

  ostringstream s;
  const size_t n = header.size();
  pair<string, const Unit*> * ptr = &header.base();
  for (long i = n - 1; i >= 0; --i)
    {
      const pair<string, const Unit*> & val = ptr[i];
      printf("%s %s", val.first.c_str(), val.second->name.c_str());
      if (i > 0)
	printf(",");
    }
  
  printf("\n");
}

void generate_grid_blackoil()
{
  set_check(); // Inicialiación de datos constantes
  set_api();
  set_rsb();
  set_yg();
  set_tsep();
  set_psep();
  set_h2s_concentration();
  set_co2_concentration();
  set_n2_concentration();
  set_nacl_concentration();
  set_pb();
  
  set_rs_corr(true); // Inicialización de correlaciones
  set_bob_corr(true);
  set_boa_corr(true);
  set_uod_corr(true);
  set_cob_corr(true);
  set_coa_corr(true);
  set_uob_corr(true);
  set_uoa_corr(true);
  set_ppchc_corr();
  set_tpchc_corr();
  set_ppcm_mixing_corr();
  set_tpcm_mixing_corr();
  set_adjustedppcm_corr();
  set_adjustedtpcm_corr();
  set_zfactor_corr();
  set_cg_corr();
  set_ug_corr();
  set_bwb_corr();
  set_bwa_corr();
  set_uw_corr();
  set_pw_corr();
  set_rsw_corr();
  set_cwb_corr();
  set_cwa_corr();
  set_sgo_corr();
  set_sgw_corr();
  report_exceptions = catch_exceptions.getValue();

  set_t_range();
  set_p_range();

  if (below_corr_arg.isSet())
    error_msg("below option is incompatible with grid option");
  if (above_corr_arg.isSet())
    error_msg("above option is incompatible with grid option");

  // cálculo de constantes para Z
  auto yghc = dcompute_noexcep(YghcWichertAziz::correlation(), true, NPAR(yg),
			       NPAR(n2_concentration), NPAR(co2_concentration),
			       NPAR(h2s_concentration));
  auto ppchc = dcompute_noexcep(ppchc_corr, true, NPAR(yghc),
				NPAR(n2_concentration), NPAR(co2_concentration),
				NPAR(h2s_concentration));
  auto ppcm = dcompute_noexcep(ppcm_mixing_corr, true, NPAR(ppchc),
			       NPAR(n2_concentration), NPAR(co2_concentration),
			       NPAR(h2s_concentration));		       
  auto tpchc = tpchc_corr->compute(check, yghc);
  auto tpcm = dcompute_noexcep(tpcm_mixing_corr, true, NPAR(tpchc),
			       NPAR(n2_concentration), NPAR(co2_concentration),
			       NPAR(h2s_concentration));
  auto adjustedppcm = dcompute_noexcep(adjustedppcm_corr, true, NPAR(ppcm),
				       NPAR(tpcm), NPAR(co2_concentration),
				       NPAR(h2s_concentration));
  auto adjustedtpcm = dcompute_noexcep(adjustedtpcm_corr, true, NPAR(tpcm),
				       NPAR(co2_concentration),
				       NPAR(h2s_concentration));
  // Fin cálculo constantes para z

  // Inicialización de listas de parámetros de correlaciones
  auto pb_pars = load_constant_parameters({pb_corr});
  auto rs_pars = load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});
  auto uod_pars = load_constant_parameters({uod_corr});
  auto bo_pars = load_constant_parameters({bob_corr, boa_corr});
  auto co_pars = load_constant_parameters({cob_corr, coa_corr});
  auto uo_pars = load_constant_parameters({uob_corr, uoa_corr});
  auto po_pars = load_constant_parameters({&PobBradley::get_instance(),
	&PoaBradley::get_instance()});
  auto ug_pars = load_constant_parameters({ug_corr});
  insert_in_container(ug_pars, npar("tpc", adjustedtpcm),
		      npar("ppc", adjustedppcm));
  auto bw_pars = load_constant_parameters({bwb_corr, bwa_corr});
  auto uw_pars = load_constant_parameters({uw_corr});
  auto pw_pars = load_constant_parameters({pw_corr});
  auto rsw_pars = load_constant_parameters({rsw_corr});
  auto cw_pars = load_constant_parameters({cwb_corr, cwa_corr});
  auto cwa_pars = load_constant_parameters({cwa_corr});
  ParList cg_pars; cg_pars.insert(npar("ppc", ppcm));
  auto sgo_pars = load_constant_parameters({sgo_corr});
  ParList sgw_pars;

  using P = pair<string, const Unit*>;
  print_csv_header(P("t", get<3>(t_values.get_first())),
		   P("pb", &pb_corr->unit),
		   P("uod", &uod_corr->unit),
		   P("p", get<3>(p_values.get_first())),
		   P("rs", &::rs_corr->unit),
		   P("co", &cob_corr->unit), 
		   P("bo", &bob_corr->unit),
		   P("uo", &uob_corr->unit),
		   P("po", &PobBradley::get_instance().unit),
		   P("zfactor", &Zfactor::get_instance()),
		   P("cg", &cg_corr->unit),
		   P("bg", &Bg::get_instance().unit),
		   P("ug", &ug_corr->unit),
		   P("pg", &Pg::get_instance().unit),
		   P("bw", &bwb_corr->unit),
		   P("uw", &uw_corr->unit),
		   P("pw", &pw_corr->unit),
		   P("rsw", &rsw_corr->unit),
		   P("cw", &cwb_corr->unit),
		   P("sgo", &sgo_corr->unit),
		   P("sgw", &sgw_corr->unit),
		   P("exception", &Unit::null_unit),
		   P("pbrow", &Unit::null_unit));

  auto rs_pb = npar("rs", rsb_par);

  FixedStack<double> row(25); // aquí van los valores. Asegure que el
			      // orden de inserción sea el mismo que
			      // para el header
  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      Correlation::NamedPar t_par = t_it.get_curr();
      VtlQuantity t_q = par(t_par);
      temperature = t_q.raw();
      CALL(Tpr, tpr, t_q, adjustedtpcm);
      auto tpr_par = NPAR(tpr);

      VtlQuantity pb_q =
	compute(pb_corr, c_pb_arg.getValue(), 1, check, pb_pars, t_par);
      auto pb = pb_q.raw();
      double next_pb = nextafter(pb, numeric_limits<double>::max());
      VtlQuantity next_pb_q = { pb_q.unit, next_pb };
      auto pb_par = npar("pb", pb_q);
      auto p_pb = npar("p", pb_q);

      auto first_p_point = p_values.get_first();
      bool first_p_above_pb = VtlQuantity(*get<3>(first_p_point),
					  get<2>(first_p_point)) > pb_q;
  
      auto uod_val = dcompute(uod_corr, check, uod_pars, t_par);

      insert_in_container(rs_pars, t_par, pb_par);
      auto rs_corr = define_correlation(pb_q, ::rs_corr, c_rs_arg.getValue(),
					m_rs_arg.getValue(),
					&RsAbovePb::get_instance());

      insert_in_container(co_pars, t_par, pb_par);
      auto co_corr =
	define_correlation(pb_q,
			   cob_corr, c_cob_arg.getValue(), m_cob_arg.getValue(),
			   coa_corr, c_coa_arg.getValue(), m_coa_arg.getValue());
      auto bo_corr =
	define_correlation(pb_q,
			   bob_corr, c_bob_arg.getValue(), m_bob_arg.getValue(),
			   boa_corr, c_boa_arg.getValue(), m_boa_arg.getValue());

      insert_in_container(uo_pars, t_par, pb_par, npar("uod", uod_val));
      auto uo_corr =
	define_correlation(pb_q,
			   uob_corr, c_uob_arg.getValue(), m_uob_arg.getValue(),
			   uoa_corr, c_uoa_arg.getValue(), m_uoa_arg.getValue());
      
      auto po_corr = define_correlation(pb_q, &PobBradley::get_instance(),
					&PoaBradley::get_instance());

      auto bw_corr = define_correlation(pb_q, bwb_corr, bwa_corr);

      auto cw_corr = define_correlation(pb_q, cwb_corr, cwa_corr);

      bo_pars.insert(t_par);
      auto bobp = compute(bob_corr, c_bob_arg.getValue(), m_bob_arg.getValue(),
			  check, bo_pars, p_pb, rs_pb);
      
      auto uobp = compute(uob_corr, c_uob_arg.getValue(), m_uob_arg.getValue(),
			  check, uo_pars, p_pb, rs_pb);

      insert_in_container(bo_pars, pb_par, NPAR(bobp));

      uo_pars.insert("uobp", uobp.raw(), &uobp.unit);

      auto pobp = dcompute(&PobBradley::get_instance(), check, po_pars,
			  rs_pb, npar("bob", bobp));

      auto bwbp = dcompute(bwb_corr, check, bw_pars, t_par, npar("p", pb_q));

      insert_in_container(po_pars, pb_par, NPAR(pobp));
      insert_in_container(ug_pars, t_par);
      insert_in_container(bw_pars, t_par, pb_par, NPAR(bwbp));
      cg_pars.insert(tpr_par);
      uw_pars.insert(t_par);
      pw_pars.insert(t_par);
      rsw_pars.insert(t_par);
      cw_pars.insert(t_par);
      cwa_pars.insert(t_par);
      sgo_pars.insert(t_par);
      sgw_pars.insert(t_par);

      size_t n = row.ninsert(t_q.raw(), pb, uod_val.raw());

      size_t i = 0;
      for (auto p_it = p_values.get_it(); p_it.has_curr(); /* nothing */)
	{
	  Correlation::NamedPar p_par = p_it.get_curr();
	  VtlQuantity p_q = par(p_par);

	  // esto es para insertar en línea las filas del punto de burbuja
	  bool pb_row = false;

	  if (p_q <= pb_q or (not (i < 2)) or first_p_above_pb)
	    p_it.next();
	  else 
	    {
	      pb_row = true;
	      p_par = npar("p", ++i == 1 ? pb_q : next_pb_q);
	      p_q = par(p_par);
	      assert(i <= 2);
	    }

	  pressure = p_q.raw();
	  CALL(Ppr, ppr, p_q, adjustedppcm);
	  auto ppr_par = NPAR(ppr);
	  auto rs = compute(rs_corr, check, p_q, rs_pars, p_par);
	  auto rs_par = npar("rs", rs, &::rs_corr->unit);
	  auto co = compute(co_corr, check, p_q, co_pars, p_par);
	  auto co_par = npar("co", co, co_corr.result_unit);
	  auto bo = compute(bo_corr, check, p_q, bo_pars, p_par, rs_par, co_par);
	  auto uo = compute(uo_corr, check, p_q, uo_pars, p_par, rs_par);
	  auto po = compute(po_corr, check, p_q, po_pars, p_par, rs_par, co_par,
			    npar("bob", bo, bo_corr.result_unit));
	  VtlQuantity z;
	  if (p_q <= pb_q)
	    z = dcompute(zfactor_corr, check, ppr_par, tpr_par);
	  auto z_par = NPAR(z);
	  auto cg = dcompute(cg_corr, check, cg_pars, ppr_par, z_par);
	  CALL(Bg, bg, t_q, p_q, z);
	  auto ug = dcompute(ug_corr, check, ug_pars, p_par, ppr_par, z_par);
	  CALL(Pg, pg, yg, t_q, p_q, z);
	  auto rsw = dcompute(rsw_corr, check, rsw_pars, p_par);
	  auto rsw_par = NPAR(rsw);
	  auto cwa = dcompute(cwa_corr, check, cwa_pars, p_par, rsw_par);
	  auto bw = compute(bw_corr, check, p_q, bw_pars, p_par, NPAR(cwa));
	  auto bw_par = npar("bw", bw, bw_corr.result_unit);
	  auto pw = dcompute(pw_corr, check, pw_pars, p_par, bw_par);
	  auto cw = compute(cw_corr, check, p_q, cw_pars, p_par, z_par, 
			    NPAR(bg), rsw_par, bw_par, NPAR(cwa));
	  CALL(PpwSpiveyMN, ppw, t_q, p_q);
	  auto uw = dcompute(uw_corr, check, uw_pars, p_par, NPAR(ppw)).raw();
	  auto sgo = dcompute(sgo_corr, check, sgo_pars, p_par).raw();
	  auto sgw = dcompute(sgw_corr, check, sgw_pars, p_par).raw();

	  size_t n = row.ninsert(p_q.raw(), rs, co, bo, uo, po, z.raw(),
				 cg.raw(), bg.raw(), ug.raw(), pg.raw(), bw,
				 uw, pw.raw(), rsw.raw(), cw, sgo, sgw);

	  assert(row.size() == 21);

	  print_row(row, pb_row);
	  row.popn(n);
	}

      row.popn(n);
      remove_from_container(rs_pars, "pb", t_par);
      remove_from_container(co_pars, "pb", t_par);
      remove_from_container(bo_pars, "bobp", "pb", t_par);
      remove_from_container(uo_pars, "uobp", "pb", "uod", t_par);
      remove_from_container(po_pars, "pb", "pobp");
      remove_from_container(ug_pars, t_par);
      remove_from_container(bw_pars, t_par, pb_par, "bwbp");
      sgo_pars.remove(t_par);
      sgw_pars.remove(t_par);
      cg_pars.remove(tpr_par);
      uw_pars.remove(t_par); 
      pw_pars.remove(t_par);
      cw_pars.remove(t_par);
      rsw_pars.remove(t_par);
    }

  if (report_exceptions)
    {
      cout << endl
	   << "Exceptions:" << endl;
      exception_list.for_each([] (const auto & s) { printf(s.c_str()); });
    }

  exit(0);
}

void generate_grid_wetgas()
{
  cout << "grid wetgas option not yet implemented" << endl;
  abort();
  exit(0);
}

void generate_grid_drygas()
{
  cout << "grid drygas option not yet implemented" << endl;
  abort();
  exit(0);
}

void generate_grid_brine()
{
  cout << "grid brine option not yet implemented" << endl;
  abort();
  exit(0);
}

void generate_grid_gascondensated()
{
  cout << "grid gacondensated option not yet implemented" << endl;
  abort();
  exit(0);
}

AHDispatcher<string, void (*)()>
grid_dispatcher("blackoil", generate_grid_blackoil,
		"wetgas", generate_grid_wetgas,
		"drygas", generate_grid_drygas,
		"brine", generate_grid_brine,
		"gascondensated", generate_grid_gascondensated);

using OptionPtr = DynList<DynList<double>> (*)();

AHDispatcher<string, OptionPtr> dispatcher("rs", generate_rs_values,
					   "bob", generate_bo_values,
					   "uob", generate_uo_values);

auto cmp_p = [] (const DynList<double> & row1, const DynList<double> & row2)
{
  const auto & p1 = row1.nth(1);
  const auto & p2 = row2.nth(1);
  if (p1 == p2)
    return row1.get_last() < row2.get_last();
  return p1 < p2;
};

auto cmp_t = [] (const DynList<double> & row1, const DynList<double> & row2)
{
  const auto & t1 = row1.get_last();
  const auto & t2 = row2.get_last();
  if (t1 == t2)
    return row1.nth(1) < row2.nth(1);
  return t1 < t2;
};

void sort(DynList<DynList<double>> & vals, SortType type)
{
  switch (type)
    {
    case SortType::Pressure:
      in_place_sort(vals, cmp_p);
      break;
    case SortType::Temperature:
      in_place_sort(vals, cmp_t);
      break;
    case SortType::Value:
            in_place_sort(vals, [] (const auto & row1, const auto & row2)
		    {
		      return row1.get_first() < row2.get_first();
		    });
	    break;
    default:
      error_msg("Invalid sort type");
      break;
  }
}

string mat_format(const DynList<DynList<double>> & vals)
{
 DynList<DynList<string>> mat = vals.maps<DynList<string>>([] (const auto & row)
  {
    return row.template maps<string>([] (auto v) { return to_string(v); }).rev();
  });
  mat.insert({"t", "p", target_name});

  return to_string(format_string(mat)); 
}

string csv_format(const DynList<DynList<double>> & vals)
{
 DynList<DynList<string>> mat = vals.maps<DynList<string>>([] (const auto & row)
  {
    return row.template maps<string>([] (auto v) { return to_string(v); }).rev();
  });
  mat.insert({"t", "p", target_name});

  return to_string(format_string_csv(mat));
}

string R_format(const DynList<DynList<double>> & mat)
{
  auto values = transpose(mat);
  auto vals = values.get_first();
  auto pressure = values.nth(1);
  auto temperature = values.get_last();

  auto min_v = numeric_limits<double>::max();
  auto max_v = numeric_limits<double>::min();

  ostringstream s;

  if (get_sort_type(sort_type.getValue()) == SortType::Temperature)
    {
      const size_t num_samples = p_range.getValue().n;
      auto min_p = numeric_limits<double>::max();
      auto max_p = numeric_limits<double>::min();
      Array<DynList<double>> p_array;
      Array<DynList<double>> v_array;
      DynList<string> suffix;
      for (auto it = get_zip_it(temperature, pressure, vals); it.has_curr();)
	{
	  auto t = it.get_curr();
	  suffix.append(to_string(get<0>(t)));
	  DynList<double> p;
	  DynList<double> v;

	  for (size_t i = 0; i < num_samples; ++i, it.next())
	    {
	      auto tt = it.get_curr();
	      auto p_val = get<1>(tt);
	      auto v_val = get<2>(tt);
	      p.append(p_val);
	      v.append(v_val);
	      min_p = min(min_p, p_val);
	      max_p = max(max_p, p_val);
	      min_v = min(min_v, v_val);
	      max_v = max(max_v, v_val);
	    }
	  p_array.append(move(p));
	  v_array.append(move(v));
	}

      DynList<string> p_names, v_names, t_names;
      for (auto it = get_zip_it(suffix, p_array, v_array);
	   it.has_curr(); it.next())
	{
	  auto t = it.get_curr();	  
	  string p_name = "p_" + get<0>(t);
	  string v_name = "v_" + get<0>(t);
	  string t_name = "\"T = " + get<0>(t) + "\"";
	  s << Rvector(p_name, get<1>(t)) << endl
	    << Rvector(v_name, get<2>(t)) << endl;
	  p_names.append(move(p_name));
	  v_names.append(move(v_name));
	  t_names.append(move(t_name));
	}
      s << "plot(0, type=\"n\", xlim=c(" << min_p << "," << max_p << "),ylim=c("
	<< min_v << "," << max_v << "))" << endl;
      for (auto it = get_enum_zip_it(p_names, v_names); it.has_curr(); it.next())
	{
	  auto t = it.get_curr();
	  s << "lines(" << get<0>(t) << "," << get<1>(t) << ",col="
	    << get<2>(t) + 1 << ")" << endl;
	}
      s << Rvector("cnames", t_names) << endl
	<< Rvector("cols", range<size_t>(1, p_array.size())) << endl
	<< "legend(\"topleft\", legend=cnames, lty=1, col=cols)";
      return s.str();
    }

  const size_t num_samples = t_range.getValue().n;
  auto min_t = numeric_limits<double>::max();
  auto max_t = numeric_limits<double>::min();
  Array<DynList<double>> t_array;
  Array<DynList<double>> v_array;
  DynList<string> suffix;
  for (auto it = get_zip_it(temperature, pressure, vals); it.has_curr();)
    {
      auto tt = it.get_curr();
      suffix.append(to_string(get<1>(tt)));
      DynList<double> t;
      DynList<double> v;

      for (size_t i = 0; i < num_samples; ++i, it.next())
	{
	  auto tt = it.get_curr();
	  auto t_val = get<0>(tt);
	  auto v_val = get<2>(tt);
	  t.append(t_val);
	  v.append(v_val);
	  min_t = min(min_t, t_val);
	  max_t = max(max_t, t_val);
	  min_v = min(min_v, v_val);
	  max_v = max(max_v, v_val);
	}
      t_array.append(move(t));
      v_array.append(move(v));
    }

  DynList<string> p_names, v_names, t_names;
  for (auto it = get_zip_it(suffix, t_array, v_array); it.has_curr(); it.next())
    {
      auto t = it.get_curr();	  
      string t_name = "t_" + get<0>(t);
      string v_name = "v_" + get<0>(t);
      string p_name = "\"P = " + get<0>(t) + "\"";
      s << Rvector(t_name, get<1>(t)) << endl
	<< Rvector(v_name, get<2>(t)) << endl;
      p_names.append(move(p_name));
      v_names.append(move(v_name));
      t_names.append(move(t_name));
    }
  s << "plot(0, type=\"n\", xlim=c(" << min_t << "," << max_t << "),ylim=c("
    << min_v << "," << max_v << "))" << endl;
  for (auto it = get_enum_zip_it(t_names, v_names); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      s << "lines(" << get<0>(t) << "," << get<1>(t) << ",col="
	<< get<2>(t) + 1 << ")" << endl;
    }
  s << Rvector("cnames", p_names) << endl
    << Rvector("cols", range<size_t>(1, t_array.size())) << endl
    << "legend(\"topleft\", legend=cnames, lty=1, col=cols)";

  return s.str();
}


int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  if (print_types.getValue())
    print_fluid_types();

  if (grid.isSet())
    grid_dispatcher.run(grid.getValue());

  set_check();
  set_api();
  set_rsb();
  set_yg();
  set_tsep();
  set_psep();
  set_pb();
  set_rs_corr();
  set_uod_corr();
  set_t_range();
  set_p_range();
  set_below_corr();
  set_above_corr();

  auto vals = dispatcher.run(target_name);

  sort(vals, get_sort_type(sort_type.getValue()));

  switch (get_output_type(output_type.getValue()))
    {
    case OutputType::mat:
      cout << mat_format(vals) << endl;
      break;
    case OutputType::csv:
      cout << csv_format(vals) << endl;
      break;
    case OutputType::json:
      error_msg("json not yet implemented");
      break;
    case OutputType::R:
      cout << R_format(vals) << endl;
      break;
    default:
      error_msg("Invalid output type");
    }
}
