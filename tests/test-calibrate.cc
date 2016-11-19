
# include <tclap/CmdLine.h>

# include <ah-comb.H>
# include <tpl_dynMapTree.H>

# include <metadata/pvt-analyse.H>
# include <metadata/pvt-calibrate.H>

using namespace std;
using namespace TCLAP;

CmdLine cmd = { "test-calibrate", ' ', "0" };

vector<string> properties =
  { "pb", "rs", "rsa", "bob", "boa", "bo", "uob", "uoa", "uo", "uod" };
ValuesConstraint<string> allowed = properties;
ValueArg<string> property =
  { "P", "property", "property", true, "", &allowed, cmd };

ValueArg<double> value = { "v", "value", "value for property", false, 0,
			   "value for the given property", cmd };
  
SwitchArg corr_list = { "l", "list", "list matching correlations", cmd };

SwitchArg corr_all = { "a", "all", "list all associated correlations", cmd };

SwitchArg corr_best = { "b", "best", "list best correlations", cmd };
  
SwitchArg set_val = { "s", "set-value", "puts given value in data set", cmd };

ValueArg<string> set_corr = { "S", "set-correlation", "set correlation",
			      false, "",
			      "set correlation for the given property", cmd };

SwitchArg plot = { "p", "plot", "generate plot data", cmd };

vector<string> compute_types = { "single", "calibrated", "both" };
ValuesConstraint<string> allowed_compute = compute_types;
ValueArg<string> compute_type = { "c", "compute-type", "compute type", false,
				 "single", &allowed_compute, cmd };

SwitchArg r = { "R", "R", "generate R script", cmd };

ValueArg<string> unit = { "u", "unit", "unit", false, "",
			  "unit for the given value", cmd };

vector<string> sort_values = { "r2", "sumsq", "mse", "distance" };
ValuesConstraint<string> allow_sort = sort_values;
ValueArg<string> sort_type = { "o", "order", "output order type", false,
			       "sumsq", &allow_sort, cmd };

SwitchArg compute = { "C", "compute", "compute correlation output", cmd };

vector<string> out_type = { "csv", "mat", "json", "R" };
ValuesConstraint<string> allowed_out_type = out_type;
ValueArg<string> output_type = { "t", "output-type", "output type", false,
				 "mat", &allowed_out_type, cmd };

MultiArg<string> corr_names = { "n", "name", "add correlation name", false,
				"add correlation name to computations", cmd };

ValueArg<string> file =
  { "f", "file", "file name", false, "", "file name", cmd };

ValueArg<double> uod =
  { "", "uod", "uod value", false, 0, "uod value", cmd };

PvtAnalyzer load_pvt_data(istream & input)
{
  return PvtAnalyzer(input);
}

void list_correlations(const DynList<const Correlation*> & l)
{
  l.for_each([] (auto p) { cout << p->call_string() << endl; });
}

auto cmp_r2 = [] (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  const CorrStat::Desc & s1 = get<2>(d1);
  const CorrStat::Desc & s2 = get<2>(d2);
  const auto & r2_1 = get<0>(s1);
  const auto & r2_2 = get<0>(s2);
  return r2_1 > r2_2;
};

auto cmp_mse = [] (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  const CorrStat::Desc & s1 = get<2>(d1);
  const CorrStat::Desc & s2 = get<2>(d2);
  const auto & mse1 = get<1>(s1);
  const auto & mse2 = get<1>(s2);
  return mse1 < mse2;
};

auto cmp_dist = [] (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  const CorrStat::Desc & s1 = get<2>(d1);
  const CorrStat::Desc & s2 = get<2>(d2);
  const auto & sigma1 = get<2>(s1);
  const auto & sigma2 = get<2>(s2);
  return sigma1 < sigma2;
};

auto cmp_sumsq = [] (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  const CorrStat::Desc & s1 = get<2>(d1);
  const CorrStat::Desc & s2 = get<2>(d2);
  const CorrStat::LFit & f1 = get<3>(s1);
  const CorrStat::LFit & f2 = get<3>(s2);
  return f1.sumsq < f2.sumsq;
};

auto get_cmp(const string & sort_type)
  -> bool (*)(const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  if (sort_type == "r2") return cmp_r2;
  if (sort_type == "sumsq") return cmp_sumsq;
  if (sort_type == "distance") return cmp_dist;
  if (sort_type == "mse") return cmp_mse;
  cout << "Invalid sort type " << sort_type << endl;
  abort();
}

DynList<double> extract_col(const PvtAnalyzer & pvt,
			    const string & set_name, const string col_name)
{
  return to_dynlist(pvt.get_data().values(set_name, col_name));
}

DynList<double> p_all(const PvtAnalyzer & pvt)
{
  
}

void error_msg(const string & msg = "Not yet implemented")
{
  cout << msg << endl;
  abort();
}

void list_correlations(const DynList<PvtAnalyzer::Desc> & l,
		       const string & sort_type)
{
  auto mat = sort(l, get_cmp(sort_type)).maps<DynList<string>>([] (auto d)
    {
      return DynList<string>({PvtAnalyzer::correlation(d)->call_string(),
	    to_string(PvtAnalyzer::r2(d)), to_string(PvtAnalyzer::mse(d)),
	    to_string(PvtAnalyzer::sigma(d)), to_string(PvtAnalyzer::sumsq(d))});
    });
  mat.insert(DynList<string>({"Correlation", "r2", "mse", "distance", "sumsq"}));
  cout << to_string(format_string(mat));
}

enum class EvalType { Single, Calibrated, Both };

EvalType get_eval_type(const string & type)
{
  if (type == "single") return EvalType::Single;
  if (type == "calibrated") return EvalType::Calibrated;
  if (type == "both") return EvalType::Both;
  error_msg("Invalid compute type " + type);
}

enum class OutputType { mat, csv, R, json };

OutputType get_output_type(const string & type)
{
  if (type == "mat") return OutputType::mat;
  if (type == "csv") return OutputType::csv;
  if (type == "R") return OutputType::R;
  if (type == "json") return OutputType::json;
  error_msg("Invalid output type " + type);
}
    
string tuned_name(const PvtAnalyzer::Desc & desc)
{
  ostringstream s;
  s << "tuned." << PvtAnalyzer::correlation(desc)->name;
  return s.str();
}

string tuned_name(const PvtAnalyzer::Desc & desc1,
		  const PvtAnalyzer::Desc & desc2)
{
  ostringstream s;
  s << "tuned." << PvtAnalyzer::correlation(desc1)->name << "."
    << PvtAnalyzer::correlation(desc2)->name;
  return s.str();
}

//                     name    tuned    c       m
using CorrDesc = tuple<string, bool, double, double>;

// TODO: hacer la misma versión pero para DefinedCorrelation

pair<DynList<CorrDesc>, DynList<DynList<double>>>
eval_correlations(const DynList<PvtAnalyzer::Desc> & l,
		  const string & set_name,
		  const string & col_name,
		  const PvtAnalyzer & pvt,
		  EvalType eval_type = EvalType::Single)
{
  if (l.is_empty())
    error_msg("Correlation list is empty");

  auto first = l.get_first();
  auto target_name = PvtAnalyzer::correlation(first)->target_name();

  auto pressure = pvt.get_data().values(set_name, "p");
  auto lab = pvt.get_data().values(set_name, col_name);

  DynList<DynList<double>> ret;
  for (auto it = get_zip_it(pressure, lab); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ret.append(DynList<double>( { get<0>(t), get<1>(t) } ));
    }

  DynList<CorrDesc> header = { make_tuple("p", false, 0, 0),
			       make_tuple(target_name, false, 0, 0) };

  switch (eval_type)
    {
    case EvalType::Single:
      for (auto itl = l.get_it(); itl.has_curr(); itl.next())
	{
	  const auto & desc = itl.get_curr();
	  const auto & values = PvtAnalyzer::values(desc);
	  auto it = values.get_it();
	  ret.mutable_for_each([&it] (auto & row)
            {
	      row.append(it.get_curr());
	      it.next();
	    });
	  header.append(make_tuple(PvtAnalyzer::correlation(desc)->name, false,
				   0, 0));
	}
	break;
    case EvalType::Calibrated:
      for (auto itl = l.get_it(); itl.has_curr(); itl.next())
	{
	  const auto & desc = itl.get_curr();
	  auto corr_ptr = PvtAnalyzer::correlation(desc);
	  auto values = pvt.get_data().tuned_compute(set_name, corr_ptr,
						     PvtAnalyzer::c(desc),
						     PvtAnalyzer::m(desc));
	  auto it = values.get_it();
	  ret.mutable_for_each([&it] (auto & row)
            {
	      row.append(it.get_curr());
	      it.next();
	    });
	  header.append(make_tuple(tuned_name(desc), true,
				   PvtAnalyzer::c(desc), PvtAnalyzer::m(desc)));
	}
      break;
    case EvalType::Both:
      for (auto itl = l.get_it(); itl.has_curr(); itl.next())
	{
	  const auto & desc = itl.get_curr();
	  auto corr_ptr = PvtAnalyzer::correlation(desc);
	  const auto & values = PvtAnalyzer::values(desc);
	  auto tuned_values = pvt.get_data().tuned_compute(set_name, corr_ptr,
							   PvtAnalyzer::c(desc),
							   PvtAnalyzer::m(desc));
	  auto vit = values.get_it();
	  auto tit = tuned_values.get_it();
	  ret.mutable_for_each([&vit, &tit] (auto & row)
			       {
				 row.append(vit.get_curr());
				 row.append(tit.get_curr());
				 vit.next();
				 tit.next();
			       });
	  header.append(make_tuple(PvtAnalyzer::correlation(desc)->name, false,
				   0, 0));
	  header.append(make_tuple(tuned_name(desc), true,
				   PvtAnalyzer::c(desc), PvtAnalyzer::m(desc)));
	}
      break;
    default:
      error_msg("Invalid evaluation type");
    }

  return make_pair(header, ret);
}

//                     name    tuned    c below   m below c above  m above
using MixedCorrDesc = tuple<string, bool, double, double, double, double>;

CorrDesc to_CorrDesc(const MixedCorrDesc & desc)
{
  return make_tuple(get<0>(desc), get<1>(desc), get<2>(desc), get<3>(desc));
}

pair<DynList<MixedCorrDesc>, DynList<DynList<double>>>
eval_correlations(const DynList<PvtAnalyzer::Desc> & lb, // below pb
		  const DynList<PvtAnalyzer::Desc> & la, // above pb
		  size_t col_idx, // respect to below set
		  const PvtAnalyzer & pvt,
		  EvalType eval_type = EvalType::Single)
{
  if (lb.is_empty())
    error_msg("Below pb correlation list is empty");

  if (la.is_empty())
    error_msg("Above pb correlation list is empty");

  auto first = lb.get_first();
  auto target_name = PvtAnalyzer::correlation(first)->target_name();

  auto pressure = pvt.get_data().values(DynList<size_t>({0, 1}), "p");
  auto lab = pvt.get_data().values(DynList<size_t>({0, 1}), col_idx);

  DynList<DynList<double>> ret;
  for (auto it = get_zip_it(pressure, lab); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ret.append(DynList<double>( { get<0>(t), get<1>(t) } ));
    }

  DynList<MixedCorrDesc> header = { make_tuple("p", false, 0, 1, 0, 1),
				    make_tuple(target_name, false, 0, 1, 0, 1) };

  switch (eval_type)
    {
    case EvalType::Single:
      for (auto itt = get_zip_it(lb, la); itt.has_curr(); itt.next())
	{
	  auto t = itt.get_curr();
	  const auto & below_desc = get<0>(t);
	  const auto & above_desc = get<1>(t);
	  const auto & below_values = PvtAnalyzer::values(below_desc);
	  const auto & above_values = PvtAnalyzer::values(above_desc);
	  {
	    auto it = below_values.get_it();
	    ret.mutable_for_each([&it] (auto & row)
              {
		row.append(it.get_curr());
		it.next();
	      });
	  }
	  {
	    auto it = above_values.get_it();
	    ret.mutable_for_each([&it] (auto & row)
              {
		row.append(it.get_curr());
		it.next();
	      });
	  }
	  header.append(make_tuple(PvtAnalyzer::correlation(below_desc)->name
				   + "_" +
				   PvtAnalyzer::correlation(above_desc)->name,
				   false, 0, 1, 0 ,1));
	}
	break;
    case EvalType::Calibrated:
      for (auto itt = get_zip_it(lb, la); itt.has_curr(); itt.next())
	{
	  auto t = itt.get_curr();
	  const auto & below_desc = get<0>(t);
	  const auto & above_desc = get<1>(t);
	  auto below_corr = PvtAnalyzer::correlation(below_desc);
	  auto above_corr = PvtAnalyzer::correlation(above_desc);

	  auto below_values =
	    pvt.get_data().tuned_compute("Below Pb", below_corr,
					 PvtAnalyzer::c(below_desc),
					 PvtAnalyzer::m(below_desc));
	  auto above_values =
	    pvt.get_data().tuned_compute("Above Pb", above_corr,
					 PvtAnalyzer::c(above_desc),
					 PvtAnalyzer::m(above_desc));

	  auto ret_it = ret.get_it();

	  for (auto it = below_values.get_it(); it.has_curr();
	       it.next(), ret_it.next())
	    ret_it.get_curr().append(it.get_curr());
	      
	  for (auto it = above_values.get_it(); it.has_curr();
	       it.next(), ret_it.next())
	    ret_it.get_curr().append(it.get_curr());

	  header.append(make_tuple(tuned_name(below_desc, above_desc), true,
				   PvtAnalyzer::c(below_desc),
				   PvtAnalyzer::m(below_desc),
				   PvtAnalyzer::c(above_desc),
				   PvtAnalyzer::m(above_desc)));
	}
      break;
    case EvalType::Both:
      for (auto itt = get_zip_it(lb, la); itt.has_curr(); itt.next())
	{
	  auto t = itt.get_curr();
	  const auto & below_desc = get<0>(t);
	  const auto & above_desc = get<1>(t);
	  auto below_corr = PvtAnalyzer::correlation(below_desc);
	  auto above_corr = PvtAnalyzer::correlation(above_desc);
	  const auto & below_values = PvtAnalyzer::values(below_desc);
	  const auto & above_values = PvtAnalyzer::values(above_desc);
	  const auto & below_tuned_values =
	    pvt.get_data().tuned_compute(0, below_corr,
					 PvtAnalyzer::c(below_desc),
					 PvtAnalyzer::m(below_desc));
	  const auto & above_tuned_values =
	    pvt.get_data().tuned_compute(1, above_corr,
					 PvtAnalyzer::c(above_desc),
					 PvtAnalyzer::m(above_desc));
	  {
	    auto vit = below_values.get_it();
	    auto tit = below_tuned_values.get_it();
	    ret.mutable_for_each([&vit, &tit] (auto & row)
				 {
				   row.append(vit.get_curr());
				   row.append(tit.get_curr());
				   vit.next();
				   tit.next();
				 });
	  }
	  {
	    auto vit = above_values.get_it();
	    auto tit = above_tuned_values.get_it();
	    ret.mutable_for_each([&vit, &tit] (auto & row)
				 {
				   row.append(vit.get_curr());
				   row.append(tit.get_curr());
				   vit.next();
				   tit.next();
				 });
	  }
	  header.append(make_tuple(PvtAnalyzer::correlation(below_desc)->name
				   + "_" +
				   PvtAnalyzer::correlation(above_desc)->name,
				   false, 0, 0, 0 ,0));
	  header.append(make_tuple(tuned_name(below_desc, above_desc), true,
				   PvtAnalyzer::c(below_desc),
				   PvtAnalyzer::m(below_desc),
				   PvtAnalyzer::c(above_desc),
				   PvtAnalyzer::m(above_desc)));
	}
      break;
    default:
      error_msg("Invalid evaluation type");
    }

  return make_pair(header, ret);
}

void process_pb(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    {
      list_correlations(pvt.rs_correlations());
      exit(0);
    }

  if (corr_list.isSet())
    {
      list_correlations(pvt.rs_valid_correlations());
      exit(0);
    }

  if (corr_best.isSet())
    {
      double pb = pvt.get_pb();
      auto l = pvt.pb_best_correlations().maps<DynList<string>>([pb] (auto t)
        {
	  int per = round(get<2>(t)/get<1>(t) * 100);
	  return DynList<string>( { get<0>(t)->call_string(),
		to_string(get<1>(t)), to_string(get<2>(t)), to_string(per) });
	});
      l.insert({"Correlation", "Value",
	    "Error (pb = " + to_string(pvt.get_pb()) + ")", "%"});
      cout << to_string(format_string(l)) << endl;
      exit(0);
    }
}

DynList<DynList<string>>
string_mat(const pair<DynList<CorrDesc>, DynList<DynList<double>>> & dmat)
{
  auto mat = dmat.second.maps<DynList<string>>([] (const auto & row)
    {
      return row.template maps<string>([] (auto v) { return to_string(v); });
    });
  mat.insert(dmat.first.maps<string>([] (auto t) { return get<0>(t); }));

  return mat;
}

DynList<DynList<string>>
string_mat(const pair<DynList<MixedCorrDesc>, DynList<DynList<double>>> & dmat)
{
  auto mat = dmat.second.maps<DynList<string>>([] (const auto & row)
    {
      return row.template maps<string>([] (auto v) { return to_string(v); });
    });
  mat.insert(dmat.first.maps<string>([] (auto t) { return get<0>(t); }));

  return mat;
}

string mat_format(const pair<DynList<CorrDesc>, DynList<DynList<double>>> & dmat)
{
  ostringstream s;
  s << to_string(format_string(string_mat(dmat)));
  return s.str();
}

string csv_format(const pair<DynList<CorrDesc>, DynList<DynList<double>>> & dmat)
{
  ostringstream s;
  s << to_string(format_string_csv(string_mat(dmat)));
  return s.str();
}

string r_format(const pair<DynList<CorrDesc>, DynList<DynList<double>>> & dmat)
{
  ostringstream s;

  auto values = transpose(dmat.second);
  for (auto it = get_zip_it(dmat.first, values); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      s << Rvector(get<0>(get<0>(t)), get<1>(t)) << endl;
    }

  auto second_name = get<0>(dmat.first.nth(1));

  s << "plot(p, " << second_name << ")" << endl;

  size_t col = 1;
  DynList<string> colnames;
  DynList<string> cols;
  for (auto it = dmat.first.get_it(2); it.has_curr(); it.next(), col++)
    {
      auto desc = it.get_curr();
      const auto & name = get<0>(desc);
      colnames.append("\"" + name + "\"");
      cols.append(to_string(col));
      s << "lines(p, " << name << ", col=" << col << ")" << endl;
    }

  s << Rvector("cnames", colnames) << endl
    << Rvector("cols", cols) << endl
    <<  "legend(\"topleft\", legend=cnames, lty=1, col=cols)" << endl;

  return s.str();
}

json to_json(const pair<CorrDesc, DynList<double>> & sample)
{
  json j;
  j["name"] = get<0>(sample.first);
  j["calibrated"] = get<1>(sample.first);
  j["c"] = get<2>(sample.first);
  j["m"] = get<3>(sample.first);
  j["values"] = to_vector(sample.second);
  return j;
}

string mat_format
(const pair<DynList<MixedCorrDesc>, DynList<DynList<double>>> & dmat)
{
  return mat_format(make_pair(dmat.first.maps<CorrDesc>([] (const auto & desc)
						  { return to_CorrDesc(desc); }),
			      dmat.second));
}

string csv_format(const pair<DynList<MixedCorrDesc>,
		  DynList<DynList<double>>> & dmat)
{
  return csv_format(make_pair(dmat.first.maps<CorrDesc>([] (const auto & desc)
					          { return to_CorrDesc(desc); }),
			      dmat.second));
}

string r_format(const pair<DynList<MixedCorrDesc>,
		DynList<DynList<double>>> & dmat)
{
  return r_format(make_pair(dmat.first.maps<CorrDesc>([] (const auto & desc)
					          { return to_CorrDesc(desc); }),
			    dmat.second));
}

json to_json(const pair<MixedCorrDesc, DynList<double>> & sample)
{
  json j;
  j["name"] = get<0>(sample.first);
  j["calibrated"] = get<1>(sample.first);
  j["c below"] = get<2>(sample.first);
  j["m below"] = get<3>(sample.first);
  j["c above"] = get<4>(sample.first);
  j["m above"] = get<5>(sample.first);
  j["values"] = to_vector(sample.second);
  return j;
}

string
json_format(const pair<DynList<CorrDesc>, DynList<DynList<double>>> & dmat)
{
  DynList<pair<CorrDesc, DynList<double>>> samples;
  auto values = transpose(dmat.second);
  for (auto it = get_zip_it(dmat.first, values); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      samples.append(make_pair(get<0>(t), get<1>(t)));
    }

  json j;
  j["data sets"] =
    to_vector(samples.maps<json>([] (const auto & s) { return to_json(s); }));
  
  return j.dump(2);
}

string
json_format(const pair<DynList<MixedCorrDesc>, DynList<DynList<double>>> & dmat)
{
  return json_format(make_pair(dmat.first.maps<CorrDesc>([] (const auto & desc)
					          { return to_CorrDesc(desc); }),
			      dmat.second));
}

DynList<const Correlation*>
read_correlation_from_command_line(PvtAnalyzer & pvt,
				   const string & target_name,
				   const string & set_name)
{
  auto valid_correlations = pvt.valid_correlations(target_name, set_name);

  DynList<const Correlation*> corr_list;
  for (auto corr_name : corr_names.getValue())
    {
      auto p = Correlation::search_by_name(corr_name);
      if (p == nullptr)
	error_msg("Correlation name " + corr_name + " not found");
      if (p->target_name() != target_name)
	error_msg("Correlation name " + corr_name + " is not for " +
		  target_name + " property");
      if (not valid_correlations.exists([&corr_name] (auto p)
					{ return p->name == corr_name; }))
	error_msg("Correlation name " + corr_name + " development range does "
		  "not fit the data associated to the given fluid");

      corr_list.append(p);
    }
  return corr_list;
}

void process_rs(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    {
      list_correlations(pvt.rs_correlations());
      exit(0);
    }

  if (corr_list.isSet())
    {
      list_correlations(pvt.rs_valid_correlations());
      exit(0);
    }

  if (corr_best.isSet())
    {
      list_correlations(pvt.rs_best_correlations(), sort_type.getValue());
      exit(0);
    }

  if (not plot.isSet())
    error_msg("Not plot option (-p) has not been set");

  if (not corr_names.isSet())
    error_msg("specific correlations have not been defined (option -n)");

  DynList<const Correlation*> corr_list =
    read_correlation_from_command_line(pvt, "rs", "Below Pb");

  auto dmat = eval_correlations(pvt.correlations_stats(corr_list, 0),
				"Below Pb", "rs", pvt,
				get_eval_type(compute_type.getValue()));

  switch (get_output_type(output_type.getValue()))
    {
    case OutputType::mat:
      cout << mat_format(dmat);
      break;
    case OutputType::csv:
      cout << csv_format(dmat);
      break;
    case OutputType::R:
      cout << r_format(dmat);
      break;
    case OutputType::json:
      cout << json_format(dmat);
      break;
    default:
      error_msg("Invalid outout type value");
    }
}

void process_rsa(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    error_msg("-" + corr_all.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (corr_list.isSet())
    error_msg("-" + corr_list.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (corr_best.isSet())
    error_msg("-" + corr_best.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (not plot.isSet())
    error_msg("Not plot option (-p) has not been set");

  if (not corr_names.isSet())
    error_msg("specific correlations have not been defined (option -n)");

  DynList<const Correlation*> below_corr_list =
    read_correlation_from_command_line(pvt, "rs", "Below Pb");

  auto above_corr_list = below_corr_list.maps<const Correlation*>([] (auto)
    {
      return &RsAbovePb::get_instance();
    });

  auto dmat = eval_correlations(pvt.correlations_stats(below_corr_list, 0),
				pvt.correlations_stats(above_corr_list, 1),
				pvt.get_data().name_index("Below Pb", "rs"), pvt,
				get_eval_type(compute_type.getValue()));

  switch (get_output_type(output_type.getValue()))
    {
    case OutputType::mat:
      cout << mat_format(dmat);
      break;
    case OutputType::csv:
      cout << csv_format(dmat);
      break;
    case OutputType::R:
      cout << r_format(dmat);
      break;
    case OutputType::json:
      cout << json_format(dmat);
      break;
    default:
      error_msg("Invalid outout type value");
    }
}

void process_bob(PvtAnalyzer & pvt)
{
  error_msg();
}

void process_boa(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_bo(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_uob(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_uoa(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_uo(PvtAnalyzer & pvt)
{
  error_msg();
}

void process_uod(PvtAnalyzer & pvt)
{
  error_msg();
}

using OptionPtr = void (*)(PvtAnalyzer&);

DynMapTree<string, OptionPtr> dispatch_tbl;

void init_dispatcher()
{
  dispatch_tbl.insert("pb", process_pb);
  dispatch_tbl.insert("rs", process_rs);
  dispatch_tbl.insert("rsa", process_rsa);
  dispatch_tbl.insert("bob", process_bob);
  dispatch_tbl.insert("boa", process_boa);
  dispatch_tbl.insert("bo", process_bo);
  dispatch_tbl.insert("uob", process_uob);
  dispatch_tbl.insert("uoa", process_uoa);
  dispatch_tbl.insert("uo", process_uo);
  dispatch_tbl.insert("uod", process_uod);
}

void dispatch_option(const string & op, PvtAnalyzer & pvt)
{
  auto command = dispatch_tbl.search(op);
  if (command == nullptr)
    error_msg("Option " + op + " not registered");
  (*command->second)(pvt);
}

int main(int argc, char *argv[])
{
  init_dispatcher();

  //cmd.xorAdd(corr_list, corr_best);
  cmd.parse(argc, argv);

  PvtAnalyzer pvt;
  if (file.isSet())
    {
      ifstream input(file.getValue());
      if (not input)
	error_msg("cannot open " + file.getValue() + " file");
      pvt = load_pvt_data(input);
    }
  else
    pvt = load_pvt_data(cin);

  dispatch_option(property.getValue(), pvt);

  return 0;
}
