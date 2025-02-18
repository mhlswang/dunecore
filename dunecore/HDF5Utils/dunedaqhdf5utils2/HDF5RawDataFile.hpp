/**
 * @file HDF5RawDataFile.hpp
 *
 * Class for reading out HDF5 files as
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef HDF5LIBS_INCLUDE_HDF5LIBS_HDF5RAWDATAFILE_HPP_
#define HDF5LIBS_INCLUDE_HDF5LIBS_HDF5RAWDATAFILE_HPP_

// DUNE-DAQ
#include "dunecore/HDF5Utils/dunedaqhdf5utils2/HDF5FileLayout.hpp"
#include "dunecore/HDF5Utils/dunedaqhdf5utils2/HDF5SourceIDHandler.hpp"
#include "dunecore/HDF5Utils/dunedaqhdf5utils2/hdf5filelayout/Structs.hpp"

#include "daqdataformats/v3_4_1/Fragment.hpp"
#include "daqdataformats/v3_4_1/TimeSlice.hpp"
#include "daqdataformats/v3_4_1/TriggerRecord.hpp"

// External Packages
#include <highfive/H5DataSet.hpp>
#include <highfive/H5File.hpp>
#include <highfive/H5Group.hpp>
#include <highfive/H5Object.hpp>
#include <nlohmann/json.hpp>

// Offline includes

#include "messagefacility/MessageLogger/MessageLogger.h"
#include "cetlib_except/exception.h"

// System
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <sys/statvfs.h>
#include <utility>
#include <variant>
#include <vector>

namespace dunedaq {

namespace hdf5libs {

/**
 * @brief HDF5RawDataFile is the class responsible
 * for interfacing the DAQ format with the HDF5 file format.
 */
class HDF5RawDataFile
{

public:
  enum
  {
    TLVL_BASIC = 2,
    TLVL_FILE_SIZE = 5
  };

  // define a record number type
  // that is a pair of the trigger record or timeslice number and sequence number
  typedef std::pair<uint64_t, daqdataformats::sequence_number_t> record_id_t; // NOLINT(build/unsigned)
  typedef std::set<record_id_t, std::less<>> record_id_set;

  // removed constructor for writing

  // constructor for reading
  explicit HDF5RawDataFile(const std::string& file_name);

  ~HDF5RawDataFile();

  std::string get_file_name() const { return m_file_ptr->getName(); }

  size_t get_recorded_size() const noexcept { return m_recorded_size; }

  std::string get_record_type() const noexcept { return m_record_type; }

  bool is_trigger_record_type() const noexcept { return m_record_type.compare("TriggerRecord") == 0; }
  bool is_timeslice_type() const noexcept { return m_record_type.compare("TimeSlice") == 0; }

  HDF5FileLayout get_file_layout() const { return *(m_file_layout_ptr.get()); }

  uint32_t get_version() const // NOLINT(build/unsigned)
  {
    return m_file_layout_ptr->get_version();
  }

public:

  template<typename T>
  T get_attribute(const std::string& name);
  template<typename T>
  T get_attribute(const HighFive::Group& grp, const std::string& name);
  template<typename T>
  T get_attribute(const HighFive::DataSet& dset, std::string name);

  std::vector<std::string> get_dataset_paths(std::string top_level_group_name = "");

  record_id_set get_all_record_ids();
  record_id_set get_all_trigger_record_ids();
  record_id_set get_all_timeslice_ids();

  std::set<uint64_t> get_all_record_numbers(); // NOLINT(build/unsigned)
  std::set<daqdataformats::trigger_number_t> get_all_trigger_record_numbers();
  std::set<daqdataformats::timeslice_number_t> get_all_timeslice_numbers();

  std::vector<std::string> get_record_header_dataset_paths();
  std::vector<std::string> get_trigger_record_header_dataset_paths();
  std::vector<std::string> get_timeslice_header_dataset_paths();

  std::string get_record_header_dataset_path(const record_id_t& rid);
  std::string get_record_header_dataset_path(const uint64_t rec_num, // NOLINT (build/unsigned)
                                             const daqdataformats::sequence_number_t seq_num = 0);
  std::string get_trigger_record_header_dataset_path(const record_id_t& rid);
  std::string get_trigger_record_header_dataset_path(const daqdataformats::trigger_number_t trig_num,
                                                     const daqdataformats::sequence_number_t seq_num = 0);
  std::string get_timeslice_header_dataset_path(const record_id_t& rid);
  std::string get_timeslice_header_dataset_path(const daqdataformats::timeslice_number_t trig_num);

  // get all fragment dataset paths
  std::vector<std::string> get_all_fragment_dataset_paths();

  // get all fragment dataset paths for given record ID
  std::vector<std::string> get_fragment_dataset_paths(const record_id_t& rid);
  std::vector<std::string> get_fragment_dataset_paths(const uint64_t rec_num, // NOLINT (build/unsigned)
                                                      const daqdataformats::sequence_number_t seq_num = 0);

  // get all fragment dataset paths for a Subsystem
  std::vector<std::string> get_fragment_dataset_paths(const daqdataformats::SourceID::Subsystem subsystem);
  std::vector<std::string> get_fragment_dataset_paths(const std::string& subsystem_name);

  // get all fragment dataset paths for a record ID and Subsystem
  std::vector<std::string> get_fragment_dataset_paths(const record_id_t& rid,
                                                      const daqdataformats::SourceID::Subsystem subsystem);
  std::vector<std::string> get_fragment_dataset_paths(const record_id_t& rid, const std::string& subsystem_name);

#if 0
  // get all fragment dataset paths for a SourceID
  std::vector<std::string> get_fragment_dataset_paths(const daqdataformats::SourceID& source_id);
  std::vector<std::string> get_fragment_dataset_paths(const daqdataformats::SourceID::Subsystem type,
  const uint32_t id); // NOLINT(build/unsigned)
  std::vector<std::string> get_fragment_dataset_paths(const std::string& typestring,
  const uint32_t id); // NOLINT(build/unsigned)

  // get a list of all the source ids from a list of fragment dataset paths
  std::set<daqdataformats::SourceID> get_source_ids(std::vector<std::string> const& frag_dataset_paths);
#endif

  //get a list of all the geo ids anywhere in the file
  std::set<uint64_t> get_all_geo_ids(); // NOLINT(build/unsigned)

  //get GeoIDs in a record
  std::set<uint64_t> get_geo_ids(const record_id_t& rid); // NOLINT(build/unsigned)
  std::set<uint64_t> get_geo_ids(const uint64_t rec_num, //NOLINT(build/unsigned)
                                 const daqdataformats::sequence_number_t seq_num = 0)
  {
    return get_geo_ids(std::make_pair(rec_num, seq_num));
  }
  std::set<uint64_t> get_geo_ids_for_subdetector(const record_id_t& rid, // NOLINT(build/unsigned)
                                                 const detdataformats::DetID::Subdetector subdet);
  std::set<uint64_t> get_geo_ids_for_subdetector(const uint64_t rec_num, //NOLINT(build/unsigned)
                                                 const daqdataformats::sequence_number_t seq_num,
                                                 const detdataformats::DetID::Subdetector subdet)
  {
    return get_geo_ids_for_subdetector(std::make_pair(rec_num, seq_num), subdet);
  }
  std::set<uint64_t> get_geo_ids_for_subdetector(const record_id_t& rid, // NOLINT(build/unsigned)
                                                 const std::string& subdet_name)
  {
    detdataformats::DetID::Subdetector subdet = detdataformats::DetID::string_to_subdetector(subdet_name);
    return get_geo_ids_for_subdetector(rid, subdet);
  }
  std::set<uint64_t> get_geo_ids_for_subdetector(const uint64_t rec_num, //NOLINT(build/unsigned)
                                                 const daqdataformats::sequence_number_t seq_num,
                                                 const std::string& subdet_name)
  {
    return get_geo_ids_for_subdetector(std::make_pair(rec_num, seq_num), subdet_name);
  }

  // get SourceIDs in a record
  std::set<daqdataformats::SourceID> get_source_ids(const record_id_t& rid);
  std::set<daqdataformats::SourceID> get_source_ids(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                    const daqdataformats::sequence_number_t seq_num = 0)
  {
    return get_source_ids(std::make_pair(rec_num, seq_num));
  }

  daqdataformats::SourceID get_record_header_source_id(const record_id_t& rid);
  daqdataformats::SourceID get_record_header_source_id(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                       const daqdataformats::sequence_number_t seq_num = 0)
  {
    return get_record_header_source_id(std::make_pair(rec_num, seq_num));
  }

  std::set<daqdataformats::SourceID> get_fragment_source_ids(const record_id_t& rid);
  std::set<daqdataformats::SourceID> get_fragment_source_ids(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                             const daqdataformats::sequence_number_t seq_num = 0)
  {
    return get_fragment_source_ids(std::make_pair(rec_num, seq_num));
  }

  // get SourceIDs for given subsystem in a record
  std::set<daqdataformats::SourceID> get_source_ids_for_subsystem(const record_id_t& rid,
                                                                  const daqdataformats::SourceID::Subsystem subsystem);
  std::set<daqdataformats::SourceID> get_source_ids_for_subsystem(const record_id_t& rid,
                                                                  const std::string& subsystem_name)
  {
    daqdataformats::SourceID::Subsystem subsys = daqdataformats::SourceID::string_to_subsystem(subsystem_name);
    return get_source_ids_for_subsystem(rid, subsys);
  }
  std::set<daqdataformats::SourceID> get_source_ids_for_subsystem(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                                  const daqdataformats::sequence_number_t seq_num,
                                                                  const daqdataformats::SourceID::Subsystem subsystem)
  {
    return get_source_ids_for_subsystem(std::make_pair(rec_num, seq_num), subsystem);
  }
  std::set<daqdataformats::SourceID> get_source_ids_for_subsystem(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                                  const daqdataformats::sequence_number_t seq_num,
                                                                  const std::string& subsystem_name)
  {
    return get_source_ids_for_subsystem(std::make_pair(rec_num, seq_num), subsystem_name);
  }

  // get SourceIDs for given fragment type in a record
  std::set<daqdataformats::SourceID> get_source_ids_for_fragment_type(const record_id_t& rid,
                                                                      const daqdataformats::FragmentType frag_type);
  std::set<daqdataformats::SourceID> get_source_ids_for_fragment_type(const record_id_t& rid,
                                                                      const std::string& frag_type_name)
  {
    daqdataformats::FragmentType frag_type = daqdataformats::string_to_fragment_type(frag_type_name);
    return get_source_ids_for_fragment_type(rid, frag_type);
  }
  std::set<daqdataformats::SourceID> get_source_ids_for_fragment_type(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                                      const daqdataformats::sequence_number_t seq_num,
                                                                      const daqdataformats::FragmentType frag_type)
  {
    return get_source_ids_for_fragment_type(std::make_pair(rec_num, seq_num), frag_type);
  }
  std::set<daqdataformats::SourceID> get_source_ids_for_fragment_type(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                                      const daqdataformats::sequence_number_t seq_num,
                                                                      const std::string& frag_type_name)
  {
    return get_source_ids_for_fragment_type(std::make_pair(rec_num, seq_num), frag_type_name);
  }

  // get SourceIDs for given subdetector in a record
  std::set<daqdataformats::SourceID> get_source_ids_for_subdetector(const record_id_t& rid,
                                                                    const detdataformats::DetID::Subdetector subdet);
  std::set<daqdataformats::SourceID> get_source_ids_for_subdetector(const record_id_t& rid,
                                                                    const std::string& subdet_name)
  {
    detdataformats::DetID::Subdetector subdet = detdataformats::DetID::string_to_subdetector(subdet_name);
    return get_source_ids_for_subdetector(rid, subdet);
  }
  std::set<daqdataformats::SourceID> get_source_ids_for_subdetector(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                                    const daqdataformats::sequence_number_t seq_num,
                                                                    const detdataformats::DetID::Subdetector subdet)
  {
    return get_source_ids_for_subdetector(std::make_pair(rec_num, seq_num), subdet);
  }
  std::set<daqdataformats::SourceID> get_source_ids_for_subdetector(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                                    const daqdataformats::sequence_number_t seq_num,
                                                                    const std::string& subdet_name)
  {
    return get_source_ids_for_subdetector(std::make_pair(rec_num, seq_num), subdet_name);
  }

#if 0
  // get SourceIDs for a system type
  std::set<daqdataformats::SourceID> get_source_ids(const daqdataformats::SourceID::Subsystem type)
  {
    return get_source_ids(get_fragment_dataset_paths(type));
  }
  std::set<daqdataformats::SourceID> get_source_ids(const std::string& typestring)
  {
    return get_source_ids(get_fragment_dataset_paths(typestring));
  }
#endif

  std::unique_ptr<char[]> get_dataset_raw_data(const std::string& dataset_path);

  std::unique_ptr<daqdataformats::Fragment> get_frag_ptr(const std::string& dataset_name);
  std::unique_ptr<daqdataformats::TriggerRecordHeader> get_trh_ptr(const std::string& dataset_name);
  std::unique_ptr<daqdataformats::TimeSliceHeader> get_tsh_ptr(const std::string& dataset_name);

  std::unique_ptr<daqdataformats::Fragment> get_frag_ptr(const record_id_t& rid,
                                                         const daqdataformats::SourceID& source_id);
  std::unique_ptr<daqdataformats::Fragment> get_frag_ptr(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                         const daqdataformats::sequence_number_t seq_num,
                                                         const daqdataformats::SourceID& source_id);
  std::unique_ptr<daqdataformats::Fragment> get_frag_ptr(const record_id_t& rid,
                                                         const daqdataformats::SourceID::Subsystem type,
                                                         const uint32_t id);     // NOLINT(build/unsigned)
  std::unique_ptr<daqdataformats::Fragment> get_frag_ptr(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                         const daqdataformats::sequence_number_t seq_num,
                                                         const daqdataformats::SourceID::Subsystem type,
                                                         const uint32_t id); // NOLINT(build/unsigned)
  std::unique_ptr<daqdataformats::Fragment> get_frag_ptr(const record_id_t& rid,
                                                         const std::string& typestring,
                                                         const uint32_t id);     // NOLINT(build/unsigned)
  std::unique_ptr<daqdataformats::Fragment> get_frag_ptr(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                         const daqdataformats::sequence_number_t seq_num,
                                                         const std::string& typestring,
                                                         const uint32_t id); // NOLINT(build/unsigned)

  std::unique_ptr<daqdataformats::Fragment> get_frag_ptr(const record_id_t& rid,
                                                         const uint64_t geo_id); // NOLINT(build/unsigned)
  std::unique_ptr<daqdataformats::Fragment> get_frag_ptr(const uint64_t rec_num, // NOLINT(build/unsigned)
                                                         const daqdataformats::sequence_number_t seq_num,
                                                         const uint64_t geo_id); // NOLINT(build/unsigned)

  std::unique_ptr<daqdataformats::TriggerRecordHeader> get_trh_ptr(const record_id_t& rid);
  std::unique_ptr<daqdataformats::TriggerRecordHeader> get_trh_ptr(const daqdataformats::trigger_number_t trig_num,
                                                                   const daqdataformats::sequence_number_t seq_num = 0)
  {
    return get_trh_ptr(std::make_pair(trig_num, seq_num));
  }

  std::unique_ptr<daqdataformats::TimeSliceHeader> get_tsh_ptr(const record_id_t& rid);
  std::unique_ptr<daqdataformats::TimeSliceHeader> get_tsh_ptr(const daqdataformats::timeslice_number_t ts_num)
  {
    return get_tsh_ptr(std::make_pair(ts_num, 0));
  }

  daqdataformats::TriggerRecord get_trigger_record(const record_id_t& rid);
  daqdataformats::TriggerRecord get_trigger_record(const daqdataformats::trigger_number_t trig_num,
                                                   const daqdataformats::sequence_number_t seq_num = 0)
  {
    return get_trigger_record(std::make_pair(trig_num, seq_num));
  }

  daqdataformats::TimeSlice get_timeslice(const daqdataformats::timeslice_number_t ts_num);
  daqdataformats::TimeSlice get_timeslice(const record_id_t& rid) { return get_timeslice(rid.first); }

  std::vector<uint64_t> get_geo_ids_for_source_id(const record_id_t& rid, // NOLINT(build/unsigned)
                                                  const daqdataformats::SourceID& source_id);

  daqdataformats::SourceID get_source_id_for_geo_id(const record_id_t& rid,
                                                    const uint64_t geo_id); // NOLINT(build/unsigned)

private:
  HDF5RawDataFile(const HDF5RawDataFile&) = delete;
  HDF5RawDataFile& operator=(const HDF5RawDataFile&) = delete;
  HDF5RawDataFile(HDF5RawDataFile&&) = delete;
  HDF5RawDataFile& operator=(HDF5RawDataFile&&) = delete;

  std::unique_ptr<HighFive::File> m_file_ptr;
  std::unique_ptr<HDF5FileLayout> m_file_layout_ptr;
  const std::string m_bare_file_name;
  const unsigned m_open_flags;

  // Total size of data being written
  size_t m_recorded_size;
  std::string m_record_type;

  // file layout reading

  void read_file_layout();
  void check_file_layout();

  // checking function
  void check_record_type(std::string);

  // unpacking groups when reading
  void explore_subgroup(const HighFive::Group& parent_group,
                        std::string relative_path,
                        std::vector<std::string>& path_list);

  // adds record-level information to caches, if needed
  void add_record_level_info_to_caches_if_needed(record_id_t rid);

  // caches of full-file and record-specific information
  record_id_set m_all_record_ids_in_file;
  HDF5SourceIDHandler::source_id_geo_id_map_t m_file_level_source_id_geo_id_map;
  std::map<record_id_t, std::set<daqdataformats::SourceID>> m_source_id_cache;
  std::map<record_id_t, daqdataformats::SourceID> m_record_header_source_id_cache;
  std::map<record_id_t, std::set<daqdataformats::SourceID>> m_fragment_source_id_cache;
  std::map<record_id_t, HDF5SourceIDHandler::source_id_path_map_t> m_source_id_path_cache;
  std::map<record_id_t, HDF5SourceIDHandler::source_id_geo_id_map_t> m_source_id_geo_id_cache;
  std::map<record_id_t, HDF5SourceIDHandler::subsystem_source_id_map_t> m_subsystem_source_id_cache;
  std::map<record_id_t, HDF5SourceIDHandler::fragment_type_source_id_map_t> m_fragment_type_source_id_cache;
  std::map<record_id_t, HDF5SourceIDHandler::subdetector_source_id_map_t> m_subdetector_source_id_cache;
};

// HDF5RawDataFile attribute getters definitions

template<typename T>
T
HDF5RawDataFile::get_attribute(const std::string& name)
{
  if (!m_file_ptr->hasAttribute(name)) {
    throw cet::exception("HDFRawDataFile.h") << "Invalid HDF5 Attribute: " << name;
  }
  auto attr = m_file_ptr->getAttribute(name);
  T value;
  attr.read(value);
  return value;
}

template<typename T>
T
HDF5RawDataFile::get_attribute(const HighFive::Group& grp, const std::string& name)
{
  if (!(grp.hasAttribute(name))) {
    throw cet::exception("HDFRawDataFile.h") << "Invalid HDF5 Attribute: " << name;
  }
  auto attr = grp.getAttribute(name);
  T value;
  attr.read(value);
  return value;
}

template<typename T>
T
HDF5RawDataFile::get_attribute(const HighFive::DataSet& dset, std::string name)
{
  if (!dset.hasAttribute(name)) {
    throw cet::exception("HDFRawDataFile.h") << "Invalid HDF5 Attribute: " << name;
  }
  auto attr = dset.getAttribute(name);
  T value;
  attr.read(value);
  return value;
}

} // namespace hdf5libs
} // namespace dunedaq

#endif // HDF5LIBS_INCLUDE_HDF5LIBS_HDF5RAWDATAFILE_HPP_

// Local Variables:
// c-basic-offset: 2
// End:
