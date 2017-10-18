
# include <gmock/gmock.h>

# include <metadata/pvt-tuner.H>

using namespace std;
using namespace testing;
using namespace std;

TEST(EmptyPvtData, simple_ops)
{
  PvtData d;

  ASSERT_TRUE(d.const_values.is_empty());
  ASSERT_TRUE(d.vectors.is_empty());
}

TEST(PvtData, add_const)
{
  PvtData data;
  data.add_const("api", 8, Api::get_instance());

  ASSERT_THROW(data.add_const("api", 9, Api::get_instance()),
	       DuplicatedConstName);

  auto cptr = data.search_const("api");
  ASSERT_NE(cptr, nullptr);
  ASSERT_EQ(cptr->name, "api");
  ASSERT_EQ(cptr->value, 8);
  ASSERT_EQ(cptr->unit_ptr, &Api::get_instance());

  Array<double> parray100 = { 100, 200, 300, 820 };
  Array<double> parray200 = { 100, 300, 800, 1120 };
  Array<double> rs100 = {16, 70, 120, 150};
  Array<double> rs200 = {20, 90, 140, 200};

  data.add_vector(100, 820, -1, -1, -1, parray100, psia::get_instance(),
		  "rs", rs100, SCF_STB::get_instance());

  ASSERT_THROW(data.add_vector(100, 820, -1, -1, -1, parray100,
			       psia::get_instance(),
			       "rs", rs100, SCF_STB::get_instance()),
	       DuplicatedVarName);
  auto vlist = data.search_vectors("rs");
  ASSERT_FALSE(vlist.is_empty());
  auto vptr = vlist.get_first();
  ASSERT_EQ(vptr->yname, "rs");
  ASSERT_EQ(vptr->t, 100);
  ASSERT_EQ(vptr->pb, 820);
  ASSERT_EQ(vptr->punit, &psia::get_instance());
  ASSERT_EQ(vptr->yunit, &SCF_STB::get_instance());
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
		      parray100, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
		      rs100, vptr->y));

  data.add_vector(200, 1120, -1, -1, -1, parray200, psia::get_instance(),
		  "rs", rs200, SCF_STB::get_instance());
  ASSERT_THROW(data.add_vector(200, 1120, -1, -1, -1, parray200,
			       psia::get_instance(),
			       "rs", rs200, SCF_STB::get_instance()),
	       DuplicatedVarName);
  vlist = data.search_vectors("rs");
  ASSERT_FALSE(vlist.is_empty());
  vptr = vlist.get_first();

  ASSERT_EQ(vptr->yname, "rs");
  ASSERT_EQ(vptr->t, 100);
  ASSERT_EQ(vptr->pb, 820);
  ASSERT_EQ(vptr->punit, &psia::get_instance());
  ASSERT_EQ(vptr->yunit, &SCF_STB::get_instance());
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      parray100, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      rs100, vptr->y));

  vptr = vlist.get_last();
  
  ASSERT_EQ(vptr->yname, "rs");
  ASSERT_EQ(vptr->t, 200);
  ASSERT_EQ(vptr->pb, 1120);
  ASSERT_EQ(vptr->punit, &psia::get_instance());
  ASSERT_EQ(vptr->yunit, &SCF_STB::get_instance());
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      parray200, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      rs200, vptr->y));
}

struct FluidTest : public Test
{
  PvtData data;
  FluidTest() // fluid5 
  {
    data.add_const("api", 8.3, Api::get_instance());
    data.add_const("co2", 0.86, MolePercent::get_instance());
    data.add_const("n2", 0.19, MolePercent::get_instance());
    data.add_const("psep", 100, psia::get_instance());
    data.add_const("rsb", 79.5, SCF_STB::get_instance());
    data.add_const("yg", 0.608, Sgg::get_instance());
    data.add_const("tsep", 100, Fahrenheit::get_instance());

    data.add_vector(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, {820, 600, 450, 300, 15},
		    psia::get_instance(), "rs",
		    {80, 62, 47, 35, 0}, SCF_STB::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, {820, 600, 450, 300, 15},
		    psia::get_instance(), "bob",
		    {1.0919, 1.0865, 1.0790, 1.0713, 1.0228},
		    RB_STB::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE,
		    {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			2000, 1800, 1600, 1400, 1200, 1000, 820},
		    psia::get_instance(), "coa",
		    {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			6.00E-06}, psia_1::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    {3000, 2800, 2600, 2400, 2200, 2000, 1800, 1600, 1400,
			1200, 1000, 820},
		    psia::get_instance(), "boa",
		    {1.0804, 1.0813, 1.0822, 1.0830, 1.0840, 1.0850,
			1.0861, 1.0872, 1.0883, 1.0895, 1.0907, 1.0919},
		    RB_STB::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE, 7500, 
		    {820, 650, 550, 450, 0}, psia::get_instance(), "uob",
		    {7500, 11350, 14000, 18120, 30000}, CP::get_instance());
    data.add_vector(125, 820, 1.0919, 30000, 7500, 
		    {3000, 2700, 2400, 2100, 1800, 1500, 1200, 1000},
		    psia::get_instance(), "uoa",
		    {12891, 11384, 10377, 9530, 8762, 8240, 7869, 7638},
		    CP::get_instance());
    data.add_vector(200, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    {3000, 2700, 2400, 2100, 1800, 1500, 1200, 900, 600, 400, 200},
		    psia::get_instance(), "uo",
		    {38.14, 36.77, 35.66, 34.42, 33.44, 32.19, 30.29,
			32.5, 37.1, 40.8, 44.5}, CP::get_instance());
    data.add_vector(300, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    {3000, 2700, 2400, 2100, 1800, 1500, 1300,
			1100, 1000, 800, 500, 200}, psia::get_instance(), "uo",
		    {370, 349.7, 335, 316.3, 302.4, 279.8, 270, 260,
			256, 278, 345.3, 404.6}, CP::get_instance());
  }
};

TEST_F(FluidTest, json)
{
  string json = data.to_json().dump();
  istringstream s(json);
  PvtData test(s);

  ASSERT_EQ(data.pb_corr, test.pb_corr);
  ASSERT_EQ(data.rs_corr, test.rs_corr);
  ASSERT_EQ(data.bob_corr, test.bob_corr);
  ASSERT_EQ(data.coa_corr, test.coa_corr);
  ASSERT_EQ(data.boa_corr, test.boa_corr);
  ASSERT_EQ(data.uod_corr, test.uod_corr);
  ASSERT_EQ(data.uob_corr, test.uob_corr);
  ASSERT_EQ(data.uoa_corr, test.uoa_corr);

  const DynList<string> const_names =
    { "api", "co2", "n2", "psep", "rsb", "yg", "tsep" };

  for (auto it = const_names.get_it(); it.has_curr(); it.next())
    {
      const string & name = it.get_curr();
      auto ptr1 = data.search_const(name);
      auto ptr2 = test.search_const(name);
      ASSERT_EQ(ptr1->name, ptr2->name);
      ASSERT_EQ(ptr1->value, ptr2->value);
      ASSERT_EQ(ptr1->unit_ptr, ptr2->unit_ptr);
    }

  constexpr double epsilon = 1e-9;
  static auto cmp = [] (double v1, double v2) { return fabs(v1 - v2) <= epsilon; };

  const DynList<string> property_names = { "rs", "bob", "coa", "boa", "uob", "uoa" };
  for (auto it = property_names.get_it(); it.has_curr(); it.next())
    {
      const string & name = it.get_curr();
      auto vlist1 = data.search_vectors(name);
      auto vlist2 = test.search_vectors(name);
      for (auto it = zip_it(vlist1, vlist2); it.has_curr(); it.next())
	{
	  auto t = it.get_curr();
	  const VectorDesc * v1 = get<0>(t);
	  const VectorDesc * v2 = get<1>(t);
	  ASSERT_NEAR(v1->t, v2->t, epsilon);
	  ASSERT_NEAR(v1->pb, v2->pb, epsilon);
	  ASSERT_NEAR(v1->bobp, v2->bobp, epsilon);
	  ASSERT_NEAR(v1->uod, v2->uod, epsilon);
	  ASSERT_NEAR(v1->uobp, v2->uobp, epsilon);
	  ASSERT_EQ(v1->punit, v2->punit);
	  ASSERT_EQ(v1->yunit, v2->yunit);
	  ASSERT_TRUE(eq(v1->p, v2->p, cmp));
	  ASSERT_TRUE(eq(v1->y, v2->y, cmp));
	}
    }
}

TEST_F(FluidTest, rm_const)
{
  auto api_ptr = data.search_const("api");
  ASSERT_NE(api_ptr, nullptr);
  ASSERT_EQ(api_ptr->name, "api");
  ASSERT_NO_THROW(data.rm_const("api"));
  ASSERT_EQ(data.search_const("api"), nullptr);
}

TEST_F(FluidTest, rm_vector)
{
  DynList<const VectorDesc*> uo_list = data.search_vectors("uo");
  ASSERT_FALSE(uo_list.is_empty());

  auto fst_uo = uo_list.get_first();
  const double temp = fst_uo->t;
  ASSERT_EQ(fst_uo->yname, "uo");
  ASSERT_NO_THROW(data.rm_vector(temp, "uo"));

  uo_list = data.search_vectors("uo");
  ASSERT_FALSE(uo_list.exists([temp] (auto vptr) { return vptr->t == temp; }));
}

// TODO 1ro debo programar inputing

TEST_F(FluidTest, get_pars)
{
  {
    const DynList<const VectorDesc*> uob_vectors = data.search_vectors("uob");
    const DynList<PvtData::Sample> l = data.get_pars(uob_vectors, "rs");
    for (auto it = zip_it(uob_vectors, l); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const VectorDesc * vptr = get<0>(t);
	const PvtData::Sample & sample = get<1>(t);
	ASSERT_TRUE(eq(vptr->p, sample.pvals));
      }
  }

  {
    const DynList<const VectorDesc*> uo_vectors = data.search_vectors("uo");
    const DynList<PvtData::Sample> l = data.get_pars(uo_vectors, "rs");
    for (auto it = zip_it(uo_vectors, l); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const VectorDesc * vptr = get<0>(t);
	const PvtData::Sample & sample = get<1>(t);
	ASSERT_TRUE(eq(vptr->p, sample.pvals));
      }
  }

  {
    const DynList<const VectorDesc*> uo_vectors = data.search_vectors("uoa");
    const DynList<PvtData::Sample> l = data.get_pars(uo_vectors, "boa");
    for (auto it = zip_it(uo_vectors, l); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const VectorDesc * vptr = get<0>(t);
	const PvtData::Sample & sample = get<1>(t);
	ASSERT_TRUE(eq(vptr->p, sample.pvals));
      }
  
    // l.for_each([] (auto & s) { cout << s << endl; });
    // data.search_vectors("uoa").for_each([] (auto &v) { cout << *v << endl; });
  } 
}

TEST_F(FluidTest, matches_with_pars)
{
  auto lbefore = data.matches_with_pars("rs");

  auto api_ptr = data.search_const("api");
  const auto api = *api_ptr;
  data.rm_const("api");

  ASSERT_EQ(data.search_const("api"), nullptr);

  auto lafter = data.matches_with_pars("rs");

  //  ASSERT_FALSE(eq(lbefore, lafter));

  for (auto it = zip_it(lbefore, lafter); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      cout << get<0>(t) << " == " << get<1>(t) << join(get<1>(t)->par_names, ",") << endl;
    }
}


TEST_F(FluidTest, can_be_applied)
{
  auto l = data.can_be_applied("rs");
  l.for_each([] (auto ptr) { cout << ptr->name << endl; });

  cout << "****************************************************************" << endl;

  Correlation::array().filter([] (auto ptr) { return ptr->target_name() == "rs"; }).
    for_each([] (auto ptr) { cout << ptr->name << endl; });
}


TEST_F(FluidTest, list_restrictions)
{

}

// TODO probar set de correlaciones

TEST_F(FluidTest, set_pb)
{

}
TEST_F(FluidTest, set_rs)
{

}
TEST_F(FluidTest, set_bob)
{

}
TEST_F(FluidTest, set_coa)
{

}
TEST_F(FluidTest, set_boa)
{

}
TEST_F(FluidTest, set_uob)
{

}
TEST_F(FluidTest, set_uoa)
{

}


TEST_F(FluidTest, get_vectors)
{

}

TEST_F(FluidTest, tp_sets)
{

}

TEST_F(FluidTest, pbapply)
{

}

TEST_F(FluidTest, rsapply)
{

}


TEST_F(FluidTest, bobapply)
{

}

TEST_F(FluidTest, coaapply)
{

}


TEST_F(FluidTest, boaapply)
{

}


TEST_F(FluidTest, uodapply)
{

}


TEST_F(FluidTest, uobapply)
{

}


TEST_F(FluidTest, uoaapply)
{

}

TEST_F(FluidTest, build_samples) // Revisar si es necesaria
{

}

TEST_F(FluidTest, iapply)
{

}

TEST_F(FluidTest, pbstats)
{

}

TEST_F(FluidTest, rsstats)
{

}

TEST_F(FluidTest, bobstats)
{

}

TEST_F(FluidTest, coastats)
{

}

TEST_F(FluidTest, boastats)
{

}

TEST_F(FluidTest, uodstats)
{

}

TEST_F(FluidTest, uobstats)
{

}

TEST_F(FluidTest, uoastats)
{

}

TEST_F(FluidTest, istats)
{

}

TEST_F(FluidTest, auto_apply)
{

}



TEST(PvtData, are_all_correlations_defined_and_missing)
{

}


