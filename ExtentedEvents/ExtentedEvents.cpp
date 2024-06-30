#include "ExtentedEvents.h"

using namespace std;

namespace Soldy {

	std::string ParseExtentedEvent(const std::string& data) {
		boost::json::object j_result;
		
		pugi::xml_document doc;
		pugi::xml_parse_result result_xml = doc.load_string(data.c_str());

		if (result_xml) {
			j_result.emplace("success", true);
			boost::json::object j_data;

			pugi::xml_node event = doc.child("event");
			string_view event_name = event.attribute("name").value();
			j_data.emplace("event_name", event_name);
			
			for (pugi::xml_node node = event.first_child(); event; node = node.next_sibling()) {
				if (node.empty()) break;
				std::string_view data_name = node.attribute("name").value();
				if (data_name == "cpu_time" || data_name == "duration" || data_name == "session_id") {
					j_data.emplace(data_name, node.child("value").text().as_llong());
				}
				else if (data_name == "database_name") {
					auto value = node.child("value").child_value();
					if (strlen(value)) j_data.emplace(data_name, value);
				}
				else if (data_name == "wait_type") {
					j_data.emplace(data_name, node.child("text").child_value());
				}
			}
			j_result.emplace("data", j_data);
		}
		else {
			j_result.emplace("success", false);
			j_result.emplace("error", result_xml.description());
		}
		return boost::json::serialize(j_result);
	}

}