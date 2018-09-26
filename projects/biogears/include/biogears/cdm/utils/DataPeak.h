/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include <biogears/exports.h>

namespace biogears {
/**
 * Utility class that looks for a peak in data
 * looking at the data one point at a time.
 *
 * Note: I wrote this for finding peaks on well behaved sinusoidal data. 
 *       I have some Ideas in the comments if the data is not as well behaved.
 */

class BIOGEARS_API DataPeak {
public:
  /**
	 * -- Number of Samples - This is how many values the peak is compared too,
	 *                        that the current peak must be greater than.
	 *					      When the this many samples are smaller than the peak,
	 *						  we have found a peak.
	 * -- epsilon - The window (below the peak if trending up, above the peak if trending down)
	 *              that a new value must be below/above in order to be considered treding away from the peak
	 *              If data is inside the window, we are in a plataue and don't do anything.
	 *              If we are going away from the peak (opposite direction) we need #samples away from the window 
	 *              to signal a peak.
	 *              If we find a point that is a new peak, we move our window and start our counting all over.
	 *
	 *  By default this class will look for max peak only
	 *  By default this class assumes data coming in is trening in the positive/up direction
	 */
  DataPeak(unsigned int samples, double epsilon);

  /** Reset Counter info */
  void Reset();

  /** 
	 * Get the last top peak value
	 */
  double LastTopPeak();

  /** 
	 * Get the last bottom peak value
	 */
  double LastBottomPeak();

  /** Check to see if we have peaked 
	 *  value is the next point of data we will check against.
	 */
  bool Peaked(double value);

protected:
  bool m_PositiveTrend;
  bool m_FindMax;
  bool m_FindMin;
  unsigned int m_Samples;
  unsigned int m_PeakCount;
  double m_Epsilon;
  double m_SamplePeak;
  double m_LastTopPeak;
  double m_LastBottomPeak;
};
}