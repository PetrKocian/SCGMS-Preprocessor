#pragma once
#include <scgms/filters/data_filter/src/descriptor.h>
#include <scgms/filters/modify_filter/src/descriptor.h>
#include <scgms/filters/print_filter/src/descriptor.h>
#include <scgms/iface/UIIface.h>bool get_filter_descriptor_by_id_combined(const GUID& id, scgms::TFilter_Descriptor& desc);scgms::SFilter create_filter_body_combined(const GUID& id, scgms::IFilter* next_filter);