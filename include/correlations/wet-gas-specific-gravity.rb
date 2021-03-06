# coding: utf-8

begin_correlation("YwgrMcCain", "WetGasSpecificGravity", "Sgg", 0.8, 1.55)
add_title("CALCULATION OF RESERVOIR WET GAS SPECIFIC GRAVITY")
add_parameter("yg", "Sgg", "Primary separator gas gravity", 0.6, 0.8)
add_parameter("yo", "Sg_do", "Condensate gravity", "Quantity<Api>(70)",  "Quantity<Api>(40)")
add_synonym("yo", "api", "api")
#add_parameter("ogrsp1", "STB_MMSCF", "Primary separator condensate gas ratio")
add_parameter("rsp1", "SCF_STB", "Primary separator producing GOR")
add_parameter("gpa", "SCF_STB", "Additional gas produced", 10, 4000)
add_parameter("veq", "SCF_STB", "Equivalent volume", 550, 5000)
add_author("McCain (Wet Gas Gravity Correction)")
add_note("Calculation of reservoir wet gas specific gravity.")
add_note("Gas mixtures that produce condensate at surface conditions may exist as a single-phase gas in the reservoir and production tubing. If reservoir/wellstream properties are desired at conditions where the mixture is single-phase, surface-gas and condensate properties must be converted to reservoir/wellstream specific gravity. This gravity should be used to estimate pseudocritical properties.")
add_note("This equation is not recommended when the total nonhydrocarbon content of the gas exceeds 25 mol%.")
add_internal_note("Producing GOR is the inverse of Condensate gas ratio.")
add_internal_note("The equation was verified by using the original reference: McCain (1991). Date: March 06 2017.")
set_hidden_blackoil_grid()
set_hidden_drygas_grid()
set_hidden_wetgas_grid()
add_ref("mcCain:1991")
end_correlation()
