#include <string>
#include "file_templates.h"

const std::string combined_descriptors_h_template =
"#include <scgms/iface/UIIface.h>\r\n"
"\r\n"
"bool get_filter_descriptor_by_id_combined(const GUID& id, scgms::TFilter_Descriptor& desc);\r\n"
"scgms::SFilter create_filter_body_combined(const GUID& id, scgms::IFilter* next_filter);\r\n"
;

const std::string comb_desc_c_includes =
"#include \"combined_descriptors.h\"\r\n"
"\r\n"
"static std::vector<scgms::TFilter_Descriptor*> all_descriptors;\r\n"
"static bool descriptors_loaded = false;\r\n"
"\r\n"
;

const std::string get_all_descs_begin =
"bool get_all_descriptors()\r\n"
"{\r\n"
"	scgms::TFilter_Descriptor* desc_begin, * desc_end; \r\n"
"\r\n"
"	bool result = "
;


const std::string get_all_descs_body =
"(&desc_begin, &desc_end) == S_OK;\r\n"
"	if (result)\r\n"
"	{\r\n"
"		for (auto iter = desc_begin; iter != desc_end; iter++)\r\n"
"		{\r\n"
"			all_descriptors.push_back(iter); \r\n"
"		}\r\n"
"	}\r\n"
"	else\r\n"
"	{\r\n"
"		return result; \r\n"
"	}\r\n"
"\r\n"
"	result = "
;

const std::string get_all_descs_end =
"(&desc_begin, &desc_end) == S_OK;\r\n"
"	if (result) {\r\n"
"		for (auto iter = desc_begin; iter != desc_end; iter++)\r\n"
"		{\r\n"
"			all_descriptors.push_back(iter); \r\n"
"		}\r\n"
"	}\r\n"
"	else\r\n"
"	{\r\n"
"		return result; \r\n"
"	}\r\n"
"\r\n"
"	if (result)\r\n"
"	{\r\n"
"		descriptors_loaded = true; \r\n"
"	}\r\n"
"	return result;\r\n"
"}\r\n"
"\r\n"
;

const std::string get_desc_by_id =
"bool get_filter_descriptor_by_id_combined(const GUID & id, scgms::TFilter_Descriptor & desc)\r\n"
"{\r\n"
"	bool result = false;\r\n"
"\r\n"
"	if (!descriptors_loaded)\r\n"
"	{\r\n"
"		if (!get_all_descriptors())\r\n"
"			return result;\r\n"
"	}\r\n"
"\r\n"
"	for (auto iter : all_descriptors)\r\n"
"	{\r\n"
"		if (iter->id == id) {\r\n"
"			memcpy(&desc, iter, sizeof(decltype(desc)));\r\n"
"			result = true;\r\n"
"			break;\r\n"
"			}\r\n"
"	}\r\n"
"\r\n"
"	return result;\r\n"
"}\r\n"
;

const std::string create_filt_comb_begin =
"scgms::SFilter create_filter_body_combined(const GUID& id, scgms::IFilter* next_filter)\r\n"
"{\r\n"
"	scgms::SFilter result; \r\n"
"	scgms::IFilter* filter; \r\n"
"\r\n"
"	if ("
;

const std::string create_filt_comb_body =
"(&id, next_filter, &filter) == S_OK)\r\n"
"	{\r\n"
"		result = refcnt::make_shared_reference_ext<scgms::SFilter, scgms::IFilter>(filter, false); \r\n"
"		return result; \r\n"
"	}\r\n"
"	if ("
;


const std::string create_filt_comb_end =
"(&id, next_filter, &filter) == S_OK)\r\n"
"	{\r\n"
"		result = refcnt::make_shared_reference_ext<scgms::SFilter, scgms::IFilter>(filter, false); \r\n"
"		return result; \r\n"
"	}\r\n"
"\r\n"
"	return result; \r\n"
"}\r\n"
;