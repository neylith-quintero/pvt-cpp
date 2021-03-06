# coding: utf-8

begin_correlation("GpaspMcCain", "AdditionalGasProduced", "SCF_STB", 10, 1300)
add_title("CALCULATION OF ADDITIONAL GAS PRODUCED FOR TWO STAGES OF SEPARATION")
add_parameter("tsep", "Fahrenheit", "Primary separator temperature", 60, 120)
add_parameter("psep", "psia", "Primary separator pressure", 100, 700)
add_parameter("yg", "Sgg", "Primary separator gas gravity", 0.6, 0.8)
add_parameter("api", "Api", "API condensate gravity", 40, 70)
add_author("McCain (Primary Separator)")
add_note("Calculation of primary separator additional gas produced (for two stages of separation).")
add_note("The additional gas produced is related to the mass of gas produced from the stock tank.")
add_note("This equation is not recommended when the total nonhydrocarbon content of the gas exceeds 25 mol%.")
add_ref("mcCain:1991")
add_internal_note("The equation was verified by using the original reference: McCain (1991). Date: March 06 2017.")
set_hidden_blackoil_grid()
set_hidden_drygas_grid()
set_hidden_wetgas_grid()
end_correlation()

################################################################

begin_correlation("Gpasp2McCain", "AdditionalGasProduced", "SCF_STB", 20, 4000)
add_title("CALCULATION OF ADDITIONAL GAS PRODUCED FOR THREE STAGES OF SEPARATION")
add_parameter("tsep", "Fahrenheit", "Primary separator temperature", 60, 120)
add_parameter("tsep2", "Fahrenheit", "Second separator temperature", 60, 120)
add_parameter("psep", "psia", "Primary separator pressure", 100, 1500)
add_parameter("yg", "Sgg", "Primary separator gas gravity", 0.6, 0.8)
add_parameter("api", "Api", "API condensate gravity", 40, 70)
add_author("McCain (Second Separator)")
add_note("Calculation of second separator additional gas produced (for three stages of separation).")
add_note("The additional gas produced is related to the mass of gas produced from the stock tank and the second separator.")
add_note("This equation is not recommended when the total nonhydrocarbon content of the gas exceeds 25 mol%.")
add_ref("mcCain:1991")
add_internal_note("The equation was verified by using the original reference: McCain (1991). Date: March 06 2017.")
set_hidden_blackoil_grid()
set_hidden_drygas_grid()
set_hidden_wetgas_grid()
end_correlation()
