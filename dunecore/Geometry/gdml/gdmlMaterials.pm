use strict;
package gdmlMaterials;

sub gen_Materials
{

# If you need to pass special materials to be additionally printed, define a
# multiline string and pass it to this function. It will be printed at the end

    my $materials = <<EOF;
<materials>
  <element name="videRef" formula="VACUUM" Z="1">  <atom value="1"/> </element>
  <element name="copper" formula="Cu" Z="29">  <atom value="63.546"/>  </element>
  <element name="beryllium" formula="Be" Z="4">  <atom value="9.0121831"/>  </element>
  <element name="bromine" formula="Br" Z="35"> <atom value="79.904"/> </element>
  <element name="hydrogen" formula="H" Z="1">  <atom value="1.0079"/> </element>
  <element name="nitrogen" formula="N" Z="7">  <atom value="14.0067"/> </element>
  <element name="oxygen" formula="O" Z="8">  <atom value="15.999"/> </element>
  <element name="aluminum" formula="Al" Z="13"> <atom value="26.9815"/>  </element>
  <element name="silicon" formula="Si" Z="14"> <atom value="28.0855"/>  </element>
  <element name="carbon" formula="C" Z="6">  <atom value="12.0107"/>  </element>
  <element name="potassium" formula="K" Z="19"> <atom value="39.0983"/>  </element>
  <element name="chromium" formula="Cr" Z="24"> <atom value="51.9961"/>  </element>
  <element name="iron" formula="Fe" Z="26"> <atom value="55.8450"/>  </element>
  <element name="nickel" formula="Ni" Z="28"> <atom value="58.6934"/>  </element>
  <element name="calcium" formula="Ca" Z="20"> <atom value="40.078"/>   </element>
  <element name="magnesium" formula="Mg" Z="12"> <atom value="24.305"/>   </element>
  <element name="sodium" formula="Na" Z="11"> <atom value="22.99"/>    </element>
  <element name="titanium" formula="Ti" Z="22"> <atom value="47.867"/>   </element>
  <element name="argon" formula="Ar" Z="18"> <atom value="39.9480"/>  </element>
  <element name="sulphur" formula="S" Z="16"> <atom value="32.065"/>  </element>
  <element name="phosphorus" formula="P" Z="15"> <atom value="30.973"/>  </element>

  <material name="Vacuum" formula="Vacuum">
   <D value="1.e-25" unit="g/cm3"/>
   <fraction n="1.0" ref="videRef"/>
  </material>

  <material name="ALUMINUM_Al" formula="ALUMINUM_Al">
   <D value="2.6990" unit="g/cm3"/>
   <fraction n="1.0000" ref="aluminum"/>
  </material>

  <material name="SILICON_Si" formula="SILICON_Si">
   <D value="2.3300" unit="g/cm3"/>
   <fraction n="1.0000" ref="silicon"/>
  </material>

  <material name="epoxy_resin" formula="C38H40O6Br4">
   <D value="1.1250" unit="g/cm3"/>
   <composite n="38" ref="carbon"/>
   <composite n="40" ref="hydrogen"/>
   <composite n="6" ref="oxygen"/>
   <composite n="4" ref="bromine"/>
  </material>

  <material name="SiO2" formula="SiO2">
   <D value="2.2" unit="g/cm3"/>
   <composite n="1" ref="silicon"/>
   <composite n="2" ref="oxygen"/>
  </material>

  <material name="Al2O3" formula="Al2O3">
   <D value="3.97" unit="g/cm3"/>
   <composite n="2" ref="aluminum"/>
   <composite n="3" ref="oxygen"/>
  </material>

  <material name="Fe2O3" formula="Fe2O3">
   <D value="5.24" unit="g/cm3"/>
   <composite n="2" ref="iron"/>
   <composite n="3" ref="oxygen"/>
  </material>

  <material name="CaO" formula="CaO">
   <D value="3.35" unit="g/cm3"/>
   <composite n="1" ref="calcium"/>
   <composite n="1" ref="oxygen"/>
  </material>

  <material name="Delrin" formula="CH2O">
    <D value="1.41" unit="g/cm3"/>
    <composite n="1" ref="carbon"/>
    <composite n="2" ref="hydrogen"/>
    <composite n="1" ref="oxygen"/>
  </material>

  <material name="MgO" formula="MgO">
   <D value="3.58" unit="g/cm3"/>
   <composite n="1" ref="magnesium"/>
   <composite n="1" ref="oxygen"/>
  </material>

  <material name="Na2O" formula="Na2O">
   <D value="2.27" unit="g/cm3"/>
   <composite n="2" ref="sodium"/>
   <composite n="1" ref="oxygen"/>
  </material>

  <material name="TiO2" formula="TiO2">
   <D value="4.23" unit="g/cm3"/>
   <composite n="1" ref="titanium"/>
   <composite n="2" ref="oxygen"/>
  </material>

  <material name="FeO" formula="FeO">
   <D value="5.745" unit="g/cm3"/>
   <composite n="1" ref="iron"/>
   <composite n="1" ref="oxygen"/>
  </material>

  <material name="CO2" formula="CO2">
   <D value="1.562" unit="g/cm3"/>
   <composite n="1" ref="iron"/>
   <composite n="2" ref="oxygen"/>
  </material>

  <material name="P2O5" formula="P2O5">
   <D value="1.562" unit="g/cm3"/>
   <composite n="2" ref="phosphorus"/>
   <composite n="5" ref="oxygen"/>
  </material>

  <material formula=" " name="DUSEL_Rock">
    <D value="2.82" unit="g/cm3"/>
    <fraction n="0.5267" ref="SiO2"/>
    <fraction n="0.1174" ref="FeO"/>
    <fraction n="0.1025" ref="Al2O3"/>
    <fraction n="0.0473" ref="MgO"/>
    <fraction n="0.0422" ref="CO2"/>
    <fraction n="0.0382" ref="CaO"/>
    <fraction n="0.0240" ref="carbon"/>
    <fraction n="0.0186" ref="sulphur"/>
    <fraction n="0.0053" ref="Na2O"/>
    <fraction n="0.00070" ref="P2O5"/>
    <fraction n="0.0771" ref="oxygen"/>
  </material>

  <material formula="Air" name="Air">
   <D value="0.001205" unit="g/cm3"/>
   <fraction n="0.781154" ref="nitrogen"/>
   <fraction n="0.209476" ref="oxygen"/>
   <fraction n="0.00934" ref="argon"/>
  </material>

  <material name="fibrous_glass">
   <D value="2.74351" unit="g/cm3"/>
   <fraction n="0.600" ref="SiO2"/>
   <fraction n="0.118" ref="Al2O3"/>
   <fraction n="0.001" ref="Fe2O3"/>
   <fraction n="0.224" ref="CaO"/>
   <fraction n="0.034" ref="MgO"/>
   <fraction n="0.010" ref="Na2O"/>
   <fraction n="0.013" ref="TiO2"/>
  </material>

  <!-- density referenced from EHN1-Cold Cryostats Technical Requirements:
       https://edms.cern.ch/document/1543254 -->
  <material name="FD_foam">
   <D value="0.09" unit="g/cm3"/>
   <fraction n="0.95" ref="Air"/>
   <fraction n="0.05" ref="fibrous_glass"/>
  </material>

  <!-- USE THIS NOW for FD and protoDUNEs - Juergen Reichenbacher (6/13/2023) -->
  <!-- Foam density is 90 kg / m^3 for the assayed protoDUNE R-PUF at SD Mines -->
  <material name="foam_protoDUNE_RPUF_assayedSample">
   <D value="0.09" unit="g/cm3"/>
   <composite n="54" ref="carbon"/>
   <composite n="60" ref="hydrogen"/>
   <composite n="4" ref="nitrogen"/>
   <composite n="15" ref="oxygen"/>
  </material>

  <!-- Foam density is 70 kg / m^3 for the 3x1x1 -->
  <material name="foam_3x1x1dp">
   <D value="0.07" unit="g/cm3"/>
   <fraction n="0.95" ref="Air"/>
   <fraction n="0.05" ref="fibrous_glass"/>
  </material>

  <!-- Copied from protodune_v4.gdml -->
  <material name="foam_protoDUNEdp">
   <D value="0.135" unit="g/cm3"/>
   <composite n="17" ref="carbon"/>
   <composite n="16" ref="hydrogen"/>
   <composite n="2" ref="nitrogen"/>
   <composite n="4" ref="oxygen"/>
  </material>

  <material name="FR4">
   <D value="1.98281" unit="g/cm3"/>
   <fraction n="0.47" ref="epoxy_resin"/>
   <fraction n="0.53" ref="fibrous_glass"/>
  </material>

  <material name="STEEL_STAINLESS_Fe7Cr2Ni" formula="STEEL_STAINLESS_Fe7Cr2Ni">
   <D value="7.9300" unit="g/cm3"/>
   <fraction n="0.0010" ref="carbon"/>
   <fraction n="0.1792" ref="chromium"/>
   <fraction n="0.7298" ref="iron"/>
   <fraction n="0.0900" ref="nickel"/>
  </material>

  <material name="Copper_Beryllium_alloy25" formula="Copper_Beryllium_alloy25">
   <D value="8.26" unit="g/cm3"/>
   <fraction n="0.981" ref="copper"/>
   <fraction n="0.019" ref="beryllium"/>
  </material>

  <material name="LAr" formula="LAr">
   <D value="1.40" unit="g/cm3"/>
   <fraction n="1.0000" ref="argon"/>
  </material>

  <material name="ArGas" formula="ArGas">
   <D value="0.00166" unit="g/cm3"/>
   <fraction n="1.0" ref="argon"/>
  </material>

  <material formula=" " name="G10">
   <D value="1.7" unit="g/cm3"/>
   <fraction n="0.2805" ref="silicon"/>
   <fraction n="0.3954" ref="oxygen"/>
   <fraction n="0.2990" ref="carbon"/>
   <fraction n="0.0251" ref="hydrogen"/>
  </material>

  <material formula=" " name="Granite">
   <D value="2.7" unit="g/cm3"/>
   <fraction n="0.438" ref="oxygen"/>
   <fraction n="0.257" ref="silicon"/>
   <fraction n="0.222" ref="sodium"/>
   <fraction n="0.049" ref="aluminum"/>
   <fraction n="0.019" ref="iron"/>
   <fraction n="0.015" ref="potassium"/>
  </material>

  <material formula=" " name="ShotRock">
   <D value="1.62" unit="g/cm3"/>
   <fraction n="0.438" ref="oxygen"/>
   <fraction n="0.257" ref="silicon"/>
   <fraction n="0.222" ref="sodium"/>
   <fraction n="0.049" ref="aluminum"/>
   <fraction n="0.019" ref="iron"/>
   <fraction n="0.015" ref="potassium"/>
  </material>

  <material formula=" " name="Dirt">
   <D value="1.7" unit="g/cm3"/>
   <fraction n="0.438" ref="oxygen"/>
   <fraction n="0.257" ref="silicon"/>
   <fraction n="0.222" ref="sodium"/>
   <fraction n="0.049" ref="aluminum"/>
   <fraction n="0.019" ref="iron"/>
   <fraction n="0.015" ref="potassium"/>
  </material>

  <material formula=" " name="Concrete">
   <D value="2.3" unit="g/cm3"/>
   <fraction n="0.530" ref="oxygen"/>
   <fraction n="0.335" ref="silicon"/>
   <fraction n="0.060" ref="calcium"/>
   <fraction n="0.015" ref="sodium"/>
   <fraction n="0.020" ref="iron"/>
   <fraction n="0.040" ref="aluminum"/>
  </material>

  <material formula="H2O" name="Water">
   <D value="1.0" unit="g/cm3"/>
   <fraction n="0.1119" ref="hydrogen"/>
   <fraction n="0.8881" ref="oxygen"/>
  </material>

  <material formula="Ti" name="Titanium">
   <D value="4.506" unit="g/cm3"/>
   <fraction n="1." ref="titanium"/>
  </material>

  <material name="TPB" formula="TPB">
   <D value="1.40" unit="g/cm3"/>
   <fraction n="1.0000" ref="argon"/>
  </material>

  <material name="Glass">
   <D value="2.74351" unit="g/cm3"/>
   <fraction n="0.600" ref="SiO2"/>
   <fraction n="0.118" ref="Al2O3"/>
   <fraction n="0.001" ref="Fe2O3"/>
   <fraction n="0.224" ref="CaO"/>
   <fraction n="0.034" ref="MgO"/>
   <fraction n="0.010" ref="Na2O"/>
   <fraction n="0.013" ref="TiO2"/>
  </material>

  <material name="Acrylic">
   <D value="1.19" unit="g/cm3"/>
   <fraction n="0.600" ref="carbon"/>
   <fraction n="0.320" ref="oxygen"/>
   <fraction n="0.080" ref="hydrogen"/>
  </material>

  <material name="NiGas1atm80K">
   <D value="0.0039" unit="g/cm3"/>
   <fraction n="1.000" ref="nitrogen"/>
  </material>

  <material name="NiGas">
   <D value="0.001165" unit="g/cm3"/>
   <fraction n="1.000" ref="nitrogen"/>
  </material>

  <material name="PolyurethaneFoam">
   <D value="0.088" unit="g/cm3"/>
   <composite n="17" ref="carbon"/>
   <composite n="16" ref="hydrogen"/>
   <composite n="2" ref="nitrogen"/>
   <composite n="4" ref="oxygen"/>
  </material>

  <material name="ProtoDUNEFoam">
   <D value="0.135" unit="g/cm3"/>
   <composite n="17" ref="carbon"/>
   <composite n="16" ref="hydrogen"/>
   <composite n="2" ref="nitrogen"/>
   <composite n="4" ref="oxygen"/>
  </material>

  <material name="LightPolyurethaneFoam">
   <D value="0.009" unit="g/cm3"/>
   <composite n="17" ref="carbon"/>
   <composite n="16" ref="hydrogen"/>
   <composite n="2" ref="nitrogen"/>
   <composite n="4" ref="oxygen"/>
  </material>

  <material name="ProtoDUNEBWFoam">
   <D value="0.021" unit="g/cm3"/>
   <composite n="17" ref="carbon"/>
   <composite n="16" ref="hydrogen"/>
   <composite n="2" ref="nitrogen"/>
   <composite n="4" ref="oxygen"/>
  </material>

  <material name="GlassWool">
   <D value="0.035" unit="g/cm3"/>
   <fraction n="0.65" ref="SiO2"/>
   <fraction n="0.09" ref="Al2O3"/>
   <fraction n="0.07" ref="CaO"/>
   <fraction n="0.03" ref="MgO"/>
   <fraction n="0.16" ref="Na2O"/>
  </material>

  <material name="Polystyrene">
   <D value="1.06" unit="g/cm3"/>
   <composite n="8" ref="carbon"/>
   <composite n="8" ref="hydrogen"/>
  </material>


EOF

my $nStrings = scalar $ARGV + 1;
for ( my $m=0; $m < $nStrings; $m++ ){
    $materials = $materials . "\n" . $_[$m] . "\n";
}

  $materials = $materials . "</materials>";


return $materials;

}


1;
