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
/**
 * @defgroup CDM_DOXYGEN Common CDM Method Definitions
 * @brief Doxygen comments for common Common Data Model methods that most CDM classes have
 * @{
 *
 * @defgroup DOXY_CDM_CLEAR Clear
 * @brief - Deletes all members.
 *
 * @defgroup DOXY_CDM_RESET Reset
 * @brief - Clears and can reallocate and initialize members.
 *
 * @defgroup DOXY_CDM_COPY Copy
 * @brief - Deep Copy except substances are shallow copies. <br>
 *          Substances are shared throughout an engine, if a subMgr is provided,<br> 
 *          this substance pointer will set to the substance pointer from the provided subMgr 
 * @details - Note the copy created will need a logger, if you provide a subMgr it will use the logger
 *            associated with the provided subMgr, if not, it will use the logger on this object <br>
 *            So if you are copying an Action used by one engine for use in another engine,<br>
*             pass in the subMgr from the other engine, so the new copy logs to that engines log
 *
 * @defgroup DOXY_CDM_COPY_FROM Copy From
 * @brief - Deep Copy provided object into this object, except substances are shallow copies. <br>
 *          Clear this contents, and copy from contents.<br>
 *          Substances are shared throughout an engine, if a subMgr is provided,<br>
 *          this substance pointer will set to the substance pointer from the provided subMgr
 *
 * @defgroup DOXY_CDM_MERGE_FROM Merge From
 * @brief - Deep Merge provided object into this object, except substances are shallow copies.<br>
 *          Copy from contents, while preserving contents this has that from does not have.<br>
 *          Substances are shared throughout an engine, if a subMgr is provided,<br>
 *          this substance pointer will set to the substance pointer from the provided subMgr
 *
 * @defgroup DOXY_CDM_GET_SCALAR Get Scalar
 * @brief - A reflextion type call that will return the Scalar associated with the string.<br>
 *          ex. GetScalar("Hematocrit") will return the SEScalarPercent object associated with Hematocrit
 * @details - Note this is an expensive call as it will string compare many times<br>
 *            This is best used, and intended for, you to dynamically prepopulate<br>
 *            a mapping data structure that will help access what you need
 *
 * @defgroup DOXY_CDM_SERIALIZE Serialize
 * @brief Serialization of data to and from bind classes
 * @{
 * @defgroup DOXY_CDM_LOAD Load
 * @brief - Clears this object then creates and sets members to data in the bind object
 *
 * @defgroup DOXY_CDM_UNLOAD Unload
 * @brief - Deletes all members.
 * @details - Caller is now the responsible for managing returned object<br>
 *            I suggest you wrap the object ptr returned with a unique_ptr*
 *
 * @defgroup DOXY_CDM_UNLOAD_TO Unload To
 * @brief - Create and push all data values to a bind class
 * @}
 *
 * @defgroup DOXY_CDM_HAS Has Property or Child object
 * @brief - Check to see if an property or child object has been allocated.
 *
 * @defgroup DOXY_CDM_GET Get Property or Child object
 * @brief - Return the Property/Child object, if it does not exist, it will be allocated.
 *
 * @defgroup DOXY_CDM_GET_VALUE Get Property Value
 * @brief - Returns the value of the Property, NaN is returned if the property does not exist.
 *
 * @}
 */