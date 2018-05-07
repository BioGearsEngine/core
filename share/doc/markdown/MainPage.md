     

# Latest Release Notes

Our latest deployment is still in a beta phase, and is intended to be an intermediate release to 
showcase the capabilities of the %BioGears&reg; Physiology Engine. 
The current version of the software includes examples of all types
of engine interfaces, but does not include all of the functionality or
physiologic systems that will be present in the final product. This
version of the software is meant to elicit feedback and enhance
community involvement in establishing end product expectations.

- - -

@insert MainPageFAQ.md

## Have more questions?

See the @ref ExtraFAQ for any other questions you may have.

- - -

## Programmatics

%BioGears is being developed under the <a href="http://www.tatrc.org/">TATRC</a> funded project:
W81XWH-13-2-0068.

Disclaimer:

> This work is supported by the US Army Medical Research and Materiel Command. The views, opinions and/or findings contained in this report are those of the author(s) and should not be construed as an official Department of the Army position, policy, or decision unless so designated by other documentation.

%BioGears is released under this @ref License.

%BioGears has @ref published papers and abstracts on several systems and clinical scenarios. 

- - -

## What's new in ver 6.3 (March 1, 2018)
The latest deployment includes the following notable updates:

- General bug fixes, system improvements, and tools/solver improvements
- Fasciculation patient event flags 
- Updated sweat methodology (fixes to ions lost in sweat)
- Updated substance and compound infusion functionality
  - Added Ringers lactate and updated 
  - Saline compound ion concentrations corrected
  - Hardened implementation 
- MuscleMass new patient data request
  - Muscle catabolism patient flag
- Added dehydration condition 
  - Implemented as scalar 0to1 representing fractional total body water lost
  - Fluid removed from patient compartments 
  - Updated patient flag for event and track body weight change (validated)
  - Added totalbodyfluidVolume as data request
  - Updated patient weight as a function of condition
- Added starvation condition
  - TimeSinceMeal determines how long since the patient's last meal 
  - Scales internal nutrient storages from validated starvation data
  - Removed ConsumeMeal condition, now replaced by starvation condition
  - Validated blood concentrations for ketones, glucose, and amino acids
  - Updated patient weight as a function of condition
- Intracellular ion transport
  - Model uses membrane potential  (see @ref TissueMethodology for details)
  - Michaelis coefficient could support more ion regulation in the future
  - Gated ion transport allows for differences between intra/extracellular spaces
- COPD now supports elevated anaerobic metabolism
- Ion transport model in the small intestine
- Updated drug library so all drugs support an effects site transport rate
- Diabetes type 1 and type 2 conditions
  - insulin resistance and insulin production effects
- Hemorrhage action now initialized with a 0-1 severity and a location (MCIS SDK example still exists)
- New drug Vasopressin
- New drug classifications in the CDM for better grouping in-code 
  - Include anesthetic, sedative, opioid, and reversal agent
  - More grouping in future work

(Interested in a previous @ref version?)

- - -
@anchor known-issues
## Known Issues and Limitations
The following are known issues with the current version of the software:
- Lack of a full sympathetic/parasympathetic nervous system
- Extravascular fluid exchange model is incomplete
- Peripheral resistance currently does not scale with core temperature
- Only tested a simulation up to 24 hours in length (No sleep model)
- The Java GUI (Toolkit) doesn't support the RenalSystemValidation scenario

- - -

## Tentative Near-Term Timeline

These are the planned updates:
- Bug fixes
- Starvation and dehydration states
- Nerve agents
- Updated ion handling and extravascular exchanges
- Improved modularity

- - -

## Credits
<b>%BioGears @ref version 6.2-beta</b>

<b><a href="http://www.ara.com/">Applied Research Associates Inc.</a></b>

*Principal Investigator:* Austin Baird, PhD

*Project Manager:* Jenn Carter

*Physiology Modeler:* Bennett Welch

*Physiology Modeler:* Matthew McDaniel

*Software Developer:* David Byrd

*Software Developer:* Brian O'Day

*Website Engineer:* Anthony Hamilton

**Consultants:**

Bryan Bergeron, MD (<b><a href="http://bryanbergeron.com/">Archetype Technologies, Inc.</a></b>)

Nicholas Moss, PhD

Stephen Mangum, PharmD

**Past Contributors:**

Aaron Bray

Jeff Webb

Rachel Clipp, PhD

Jerry Heneghan

Yeshitila Gebremichael, PhD

Zack Swarm

Pat Russler

Beth Smith

Paul Rutledge

Federico Menozzi

Alex Somers

Katie Carter

Cassidy Limer

<b><a href="https://pharmacy.unc.edu/">UNC Eshelman School of Pharmacy:</a></b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Alexander Tropsha, PhD<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Kimberly Brouwer, PhD<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Denise Rhoney, PharmD<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Eugene Muratove, PhD<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Daniel Gonzalez, PharmD, PhD <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Alexander Golbraikh, PhD<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Vadryn Pierre, PharmD<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Nilay Thakakkar, PhD<br>

**Acknowledgement:**

The %BioGears team would like to thank the following individuals for providing programmatic guidance and oversight for the U.S. Government on this project:  
	
Jan Harris, PhD, RN<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Director, Joint Program Committee-1*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *U.S. Army Medical Research and Materiel Command (USAMRMC)*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Fort Detrick, Maryland*

Kevin Kunkler, MD<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Portfolio Manager, Joint Program Committee-1*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *U.S. Army Medical Research and Materiel Command (USAMRMC)*<br> 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Fort Detrick, Maryland*<br>

J. Harvey Magee<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Manager, Medical Modeling and Simulation Innovation Center (MMSIC)*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Telemedicine & Advanced Technology Research Center (TATRC)*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *US Army Medical Research & Materiel Command (USAMRMC)*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Fort Detrick, Maryland*<br>

Geoffrey T. Miller, MS<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Assistant Professor, Eastern Virginia Medical School*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Research Scientist, Medical Modeling and Simulation Innovation Center (MMSIC)*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Telemedicine & Advanced Technology Research Center (TATRC)*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *US Army Medical Research & Materiel Command (USAMRMC)*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Fort Detrick, Maryland*<br>

Thomas B. Talbot, MD<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Principal Medical Expert - USC Institute for Creative Technologies*<br> 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Associate Research Professor of Medical Education - Keck School of Medicine, USC*<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Playa Vista, CA*<br>

**Software Tools:**

%BioGears leverages the following: @ref Credits.

---

## Dedication

This software is dedicated to **N. Ty Smith, M.D.**, physician, professor, mentor, friend, and founding director of the Pacific Academy of Ecclesiastical Music (PACEM).

Dr. Smith was born in Iowa and graduated from Harvard College and Harvard Medical School. Dr. Smith served on the faculties at Stanford Medical Center and the University of California at San Diego. He was a Visiting Professor at the University of Washington, Institute of Medical Physics in Holland, University of Wisconsin, and University of Otago in New Zealand. Dr. Smith also served at Children's Medical Center in Boston, Massachusetts, Massachusetts General Hospital, U.S. Naval Hospital in Portsmouth, Virginia, U.S. Veterans Administration Hospital in San Diego, and Dunedin Hospital in New Zealand.

Dr. Smith, along with Ken Starko M.Sc., created "Body Simulation" in the 1990s. Body Simulation models and their interfaces were used for pharmacologic experimentation, testing, teaching, and training by device manufacturers, pharmaceutical companies, professional associations, and government agencies. %BioGears builds directly from this vision and legacy.

It was Dr. Smith's fervent wish that his work continue long into the future to advance scientific discovery, improve the safety of healthcare, and ultimately save lives.
 
*The %BioGears Team*<br>
*Raleigh North Carolina, September 2014*


@page errors Errors

@page events Events

