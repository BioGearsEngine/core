#ifndef BIOGEARS_CDM_UTILS_UNITCONVERSION_UCEDefs_H 
#define BIOGEARS_CDM_UTILS_UNITCONVERSION_UCEDefs_H 

OPEN_BIOGEARS_NAMESPACE
// Prefix format:
// PREFIX <name> <symbol> <conversion-factor>  
// [note that symbol is single char only]
//
// Quantity formats: 
// 1) QUANTITY FUNDAMENTAL <name> [20LOG]
// 2) QUANTITY DERIVED <name> [20LOG] <expansion>
//	<expansion> is a string representation of a complex unit. Juxtaposed symbols must be whitespace-separated. 
//		Symbols must already be defined
//    	Note that each quantity definition implicitly defines an anonymous, "base unit" for that type. 
//		The base unit refers to any subsequently-defined unit that has a conversion-factor of 1.0. For fundamental
//       types, this is also the unit's bigness. For derived types, the bigness of any unit is its conversion
//		factor times the bigness of the derived type's expansion. The unit conversion engine does not require
//		the existence of a named unit whose conversion factor is 1.0, though for fundamental types, there is 
//		really no reason for there not to be one.
//	[20LOG] is an optional flag that indicates that this quantity should obey the "20 Log Rule",
//		for decibels, as explained in http://en.wikipedia.org/wiki/Decibel
//
// Unit formats 
// 1) UNIT <quantity-name> <name> <symbol> <conversion-factor> <expansion>  	{ALLPREFIXES/NOPREFIXES}  [<bias>]
// 2) UNIT <quantity-name> <name> <symbol> <conversion-factor> <expansion>	{GOODPREFIXES/BADPREFIXES} <prefix-char-seq>  [<bias>]
// 	[bias] is at the end because it is optional and seldom used (e.g. temperature)
// 	<prefix-char-seq> is a sequence of prefix characters that are either allowed or forbidden
//	<quantity-name> is the name of the previously-defined quantity type to which this unit applies.
// 	<expansion> is a complex unit with respect to which the current unit's conversion factor and bias are defined, or "*" 
// 		if wrt the base unit. A note on biases: Whereas conversion factors are
//		relative, and the choice of base unit is largely arbitrary as far as conversion factors are concerned,
//		the same is not true with biases. In order for quantity type conversions to be done properly
//		(e.g. Temperature to Energy and vice versa), the base unit must be an absolute scale (e.g.
//		(Kelvin or Rankine). Selecting Celsius as the base unit, and specifying biases relative to
//		it, may work for simple conversions from one temperature to another, but will not yield 
//		correct results when converting between quantity types. This restriction does not 
//		preclude defining such a unit's bias and conversion factor in terms of some other
//		non-base unit, so long as the definition chain ends with a unit of absolute scale. Note that
//		the specified bias value is the value of the current unit when the target unit has the
//		value zero.
//
// Quantity type mapping format:
// TYPEMAP <to-quantity> <from-quantity> <from-exponent> <mapping-unit> 
//	TYPEMAP allows the conversion between disparate quantity types. Such conversions are almost
//	always context dependent. For example, one can convert frequency to energy in the context of
//	electromagnetic radiation by multiplying the frequency (raised to the first power) by Planck's 
//	constant (raised to the first power).
//	<to-quantity> is the name of the quantity being converted to (e.g. Energy)
//	<from-quantity> is the name of the quantity type being converted from (e.g. Frequency).
//	<from-exponent> is the power that the original value needs to be raised to. (In the case of E = h nu,
//		this power is 1.0, but some conversions might involve an inverse relationship, in which
//		case the power might be -1.0.)
//	<mapping-unit> is an arbitrary compound unit string representing 
//		the physical constant or constants, such as "c", or "h", or "c^2", or "c h".
//	Note that there is no need to define an inverse typemap in the other direction. This is computed automatically.

// A note on decibels
// Compled units may be preceded with "dB " to signify values in decibels relative to the unit which follows.
// E.g., you can use a unit like "dB mW", or "dB V/m". 

constexpr const char * UCEDEFS_TEXT = R"UCEDEFS( 
PREFIX	yotta	Y	1.0e24
PREFIX  zetta	Z	1.0e21
PREFIX	exa	E	1.0e18
PREFIX	peta	P	1.0e15
PREFIX  tera	T	1.0e12
PREFIX	giga	G	1.0e9
PREFIX	mega	M	1.0e6
PREFIX	kilo	k	1.0e3
PREFIX	hecto	h	1.0e2
PREFIX	deci	d	1.0e-1
PREFIX	centi	c	1.0e-2
PREFIX	milli	m	1.0e-3
PREFIX	micro	u	1.0e-6
PREFIX	nano	n	1.0e-9
PREFIX	pico	p	1.0e-12
PREFIX	femto	f	1.0e-15
PREFIX	atto	a	1.0e-18
PREFIX	zepto	z	1.0e-21
PREFIX	yocto	y	1.0e-24

QUANTITY	FUNDAMENTAL	Time		
UNIT	Time		second		s 	1.0		*	GoodPrefixes munpfazy 
UNIT	Time		minute		min	60.0		s	NoPrefixes
UNIT	Time		hour		hr	60.0		min	NoPrefixes
UNIT	Time		day		day	24.0		hr	NoPrefixes
UNIT	Time		year		yr	365.242199	day	NoPrefixes
UNIT	Time		fortnight	fortnight	14	day	NoPrefixes

QUANTITY	FUNDAMENTAL	Distance	
UNIT	Distance	meter		m	1.0		*	BadPrefixes MGTPEZY
UNIT	Distance	inch		in	2.54		cm	NoPrefixes
UNIT	Distance	mil		mil	0.001		in	NoPrefixes
UNIT	Distance	foot		ft	12		in	GoodPrefixes k
UNIT	Distance	yard		yd	3		ft	NoPrefixes
UNIT	Distance	mile		mi	5280		ft	NoPrefixes
UNIT	Distance	nautical-mile	nmi	1852		m	NoPrefixes
UNIT	Distance	cubit		cubit	18		in	NoPrefixes
UNIT	Distance	furlong		furlong	0.125		mi	NoPrefixes
UNIT	Distance	chain		chain	22		yd	NoPrefixes
UNIT	Distance	rod		rod	16.5	        ft	NoPrefixes
UNIT	Distance	fathom		fathom	6		ft	NoPrefixes

QUANTITY	FUNDAMENTAL	Osmole		
UNIT	Osmole		osmole		Osm 	1.0		*	GoodPrefixes m 

QUANTITY	DERIVED		PlaneAngle		m/m
UNIT	PlaneAngle	pi		pi		3.1415926535897932384626433832795	*		NoPrefixes
UNIT	PlaneAngle	degpercyc	degpercyc	360.0					* 		NoPrefixes
UNIT	PlaneAngle	degfac		degfac		60.0					*		NoPrefixes
UNIT	PlaneAngle	radian		rad		0.5					1/pi 		GoodPrefixes cmu
UNIT	PlaneAngle	degree		deg		1.0					1/degpercyc 	NoPrefixes
UNIT	PlaneAngle	minute-arc	minarc		1.0					deg/degfac	NoPrefixes
UNIT	PlaneAngle	second-arc	secarc		1.0					minarc/degfac	NoPrefixes

QUANTITY	DERIVED		Frequency	1/s
UNIT	Frequency	hertz		Hz	1.0		*	GoodPrefixes kMGT

QUANTITY	FUNDAMENTAL	AmountOfSubstance
UNIT	AmountOfSubstance	mole		mol	1.0		*	AllPrefixes

QUANTITY	DERIVED		InverseAmount	1/mol 
UNIT	InverseAmount	AvogadrosConstant	Na	6.02214179E23	*	NoPrefixes

UNIT	AmountOfSubstance	count		ct		1.0			Na^-1	NoPrefixes
UNIT	AmountOfSubstance	dozen		doz		12			ct		NoPrefixes

QUANTITY	FUNDAMENTAL	Mass
UNIT	Mass	gram		g		1.0		*		AllPrefixes
UNIT	Mass	UnifiedAMU	u		1.0		g/mol Na	NoPrefixes
UNIT	Mass	dalton		Da		1.0		u		GoodPrefixes k
UNIT	Mass	AMU			amu		1.0		u		NoPrefixes
UNIT	Mass	ounce		oz		28.349523125	g		NoPrefixes
UNIT	Mass	pound		lb		16	 	oz		NoPrefixes
UNIT	Mass	stone		stone	14		lb		NoPrefixes
UNIT	Mass	ton			ton		2000.0		lb		NoPrefixes

QUANTITY	FUNDAMENTAL	20LOG Current
UNIT	Current	ampere		A	1.0		*	AllPrefixes

QUANTITY	FUNDAMENTAL	LuminousIntensity	
UNIT	LuminousIntensity	candela		cd	1.0		*	AllPrefixes

QUANTITY 	FUNDAMENTAL	Temperature
UNIT	Temperature	DegreesRankine		degR	1.0			*	AllPrefixes	0.0
UNIT	Temperature	Kelvin		K		1.8			degR	AllPrefixes	0.0
UNIT	Temperature	DegreesCelsius		degC	1.0			K	AllPrefixes	-273.15
UNIT	Temperature	DegreesFahrenheit	degF	0.5555555555555		degC	NoPrefixes	32.0

QUANTITY	DERIVED		Area		m^2
UNIT	Area		acre		acre	4046.85642	*	NoPrefixes
UNIT	Area		are		are	100		*	NoPrefixes
UNIT	Area		decare		decare	10		are	NoPrefixes
UNIT	Area		barn	b	100		fm^2	NoPrefixes

QUANTITY	DERIVED		Volume		m^3
UNIT	Volume		liter		l	0.001		*	GoodPrefixes mu
UNIT	Volume		Liter		L	1.0		l	GoodPrefixes mud
UNIT	Volume		gallon		gal	3.7854118	l	NoPrefixes
UNIT	Volume		quart		qt	0.25		gal	NoPrefixes
UNIT	Volume		pint		pt	0.5		qt	NoPrefixes
UNIT	Volume		cubic-centimeter	cc	1.0		cm^3	NoPrefixes
UNIT	Volume		cubic-foot	cf	1.0	ft^3	NoPrefixes
UNIT	Volume		peck	peck	2	gal		NoPrefixes
UNIT	Volume		bushel	bushel	4	peck	NoPrefixes
UNIT	Volume		hogshead	hhd	63	gal		NoPrefixes
UNIT	Volume		barrel		brl	31.5	gal	NoPrefixes

QUANTITY	DERIVED		Force		kg m/s^2
UNIT	Force		newton		N	1.0		*	AllPRefixes
UNIT	Force		dyne		dyn	1.0e-5		N	AllPrefixes
UNIT	Force		pound-force	lbf	4.44822162	N	NoPrefixes

QUANTITY	DERIVED		Energy		N m
UNIT	Energy		joule		J	1.0		*	BadPRefixes c
UNIT	Energy		erg		erg	1.0e-7		J	AllPrefixes
UNIT	Energy		calorie		cal	4.184		J	AllPrefixes
UNIT	Energy		Calorie		Cal	1.0		kcal	AllPrefixes
UNIT 	Energy		tonne-of-TNT	t	1.0		Gcal	GoodPrefixes umkMG
UNIT    Energy      BritishThermalUnit		BTU		1055.05585		J		NoPrefixes

QUANTITY	DERIVED		Power		J/s
Unit	Power		watt		W	1.0		*	AllPrefixes
Unit	Power		horsepower	hp	745.699872	W	GoodPRefixes k

QUANTITY	DERIVED	20LOG	Speed		m/s
UNIT	Speed		lightspeed	c	2.99792458e+8	*	NoPrefixes
UNIT	Speed		speedoflight	lt	1.0 		c	NoPrefixes
UNIT	Speed		milesperhour	mph	1.0		mi/hr	NoPrefixes
UNIT	Speed		knots		kn	1.0		nmi/hr	NoPrefixes

QUANTITY	DERIVED		Acceleration	m/s^2
UNIT	Acceleration	gravity		grav	9.8		*	NoPrefixes

QUANTITY	DERIVED		Pressure	N/m^2
UNIT	Pressure	pascal		Pa	1.0		*		AllPrefixes
UNIT	Pressure	bar		bar	1.0e+5		Pa		AllPrefixes
UNIT	Pressure	torr		Torr	133.322368	Pa		AllPrefixes
UNIT	Pressure	atmosphere	atm	760		Torr		AllPrefixes
UNIT	Pressure	psi		psi	1.0		lbf/in^2 	NoPrefixes
UNIT	Pressure	mmHg	mmHg	1.0 Torr	NoPrefixes
UNIT	Pressure	cmH2O	cmH2O	98.0665 Pa	NoPrefixes

QUANTITY	DERIVED	20LOG	ElectricCharge	A s
UNIT	ElectricCharge	coulomb			C	1.0		*	AllPrefixes
UNIT	ElectricCharge	elementary-charge	e	1.60217646E-19	C	NoPrefixes

QUANTITY	DERIVED	20LOG	ElectricPotential	W/A
UNIT	ElectricPotential	volt		V	1.0		*	AllPrefixes

UNIT	Energy		electron-volt	eV	1.0 	e V	AllPrefixes

QUANTITY	DERIVED		Capacitance	C/V
UNIT	Capacitance	farad		F	1.0		*	AllPrefixes

QUANTITY	DERIVED		Resistance	V/A
UNIT	Resistance	ohm		ohm	1.0		*	AllPrefixes
UNIT	Resistance	Z0		Z0	120		pi ohm	NoPrefixes

QUANTITY	DERIVED	ThermalResistanceArea	m^2 L/W
UNIT	ThermalResistanceArea rsi	rsi		1.0		*	NoPrefixes
UNIT	ThermalResistanceArea clo	clo		0.155		rsi	NoPrefixes
UNIT	ThermalResistanceArea rValue	rValue		0.1761101838		rsi	NoPrefixes
UNIT	ThermalResistanceArea tog	tog		0.1		rsi	NoPrefixes

QUANTITY	DERIVED		Conductance	A/V
UNIT	Conductance	siemens		S	1.0		*	AllPrefixes

QUANTITY	DERIVED	20LOG	MagneticFlux	V s
UNIT	MagneticFlux	weber		Wb	1.0		*	AllPrefixes

QUANTITY	DERIVED	20LOG	MagneticFluxDensity	Wb/m^2
UNIT	MagneticFluxDensity	tesla		T	1.0		*	AllPrefixes
UNIT	MagneticFluxDensity	gauss		G	0.0001		T	AllPrefixes

QUANTITY	DERIVED		Inductance	Wb/A
UNIT	Inductance	henry		H	1.0		*	AllPrefixes

QUANTITY	DERIVED		SolidAngle	m^2/m^2
UNIT	SolidAngle	steradian	sr	.25		1/pi 	GoodPrefixes cmun

QUANTITY	DERIVED		LuminousFlux	cd sr
UNIT	LuminousFlux	lumen		lm	1.0		*	AllPrefixes

QUANTITY	DERIVED		Illuminance	lm/m^2
UNIT	Illuminance	lux		lx	1.0		*	AllPrefixes
UNIT	Illuminance	footcandle	fc	1.0	lm/ft^2	AllPrefixes

QUANTITY	DERIVED		Luminance cd/m^2
UNIT	Luminance	nit		nit	1.0		*	AllPrefixes
UNIT	Luminance	stilb	sb	1.0		cd/cm^2	AllPrefixes
UNIT	Luminance	lambert	Lb	1.0		sb/pi	AllPrefixes
UNIT	Luminance	footlambert	fL	1.0		cd/pi ft^2	AllPrefixes

QUANTITY	DERIVED		ActivityRadionuclide	1/s
UNIT	ActivityRadionuclide	becquerel	Bq	1.0		*	AllPrefixes

QUANTITY	DERIVED		Radiation-Dose	J/kg
UNIT	Radiation-Dose	gray		Gy	1.0		*	AllPrefixes
UNIT	Radiation-Dose	rad			rd	0.01	Gy	NoPrefixes

QUANTITY	DERIVED		Action		J s
UNIT	Action		planck's-constant h	6.6260689633E-34 *	NoPrefixes
QUANTITY	DERIVED		AngularAction	h/pi
UNIT	AngularAction	dirac's-constant  hbar	1.0		*	NoPrefixes

QUANTITY	DERIVED		MolesOfMass	mol g

QUANTITY 	DERIVED	20LOG	ElectricFieldStrength	V/m
QUANTITY	DERIVED		PowerDensity		W/cm^2

QUANTITY	DERIVED		BoltzmannQuantity	J/K
UNIT	BoltzmannQuantity	BoltzmannConstant	kB	1.3806503E-23 *	NoPrefixes

TYPEMAP 	Energy		Mass		1	c^2	
TYPEMAP		Energy		Frequency	1	h	
TYPEMAP		Distance	Frequency	-1	c	
TYPEMAP		Energy		Distance	-1	h c	
TYPEMAP		Mass		MolesOfMass	1	Na	
TYPEMAP		PowerDensity	ElectricFieldStrength	2	1/Z0
TYPEMAP		Energy		Temperature	1	kB

)UCEDEFS";

CLOSE_BIOGEARS_NAMESPACE
#endif //BIOGEARS_CDM_UTILS_UNITCONVERSION_UCEDefs_H 
