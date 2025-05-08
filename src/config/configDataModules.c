#include <string.h>
#include "../../include/configHelpers.h"
#include "../../include/configDataModules.h"
#include "../../include/node.h"
//#include "../../include/utilsNameValue.h"

void config_data_modules(json_t *dataModules) {

    /*
    if (json_is_array(dataModules)) {
        
        size_t module_index, component_index, tag_index, nv_index;
        json_t  *element, *sub_element, *nv_element, *m_tagPathName,
                *c_tagPathName, *s_name, *s_value, *start, *verbose,
                *moduleId, *components, *settings, *threadSleepMilliseconds,
                *t_element, *tags, *tag_id, *tag_description, *tag_type;
        
        // DataModules
        json_array_foreach(dataModules, module_index, element) {
            m_tagPathName = json_object_get(element, "tagPathName");
            start = json_object_get(element, "start");
            verbose = json_object_get(element, "verbose");
            moduleId = json_object_get(element, "moduleId");
            components = json_object_get(element, "components");
            threadSleepMilliseconds = json_object_get(element, "threadSleepMilliseconds");
     
            // Set Module @ module_index
            DataModule *dest = &rpiNode.data[module_index];
            dest->flag = 1;
            dest->start = get_true_false(0, start);
            dest->verbose = get_true_false(0, verbose);
            dest->moduleId = get_integer(dest->moduleId, moduleId);
            dest->threadSleepMilliseconds = get_integer(1, threadSleepMilliseconds);
            rpiNode.data_count++;
            SAFE_STRCPY(dest->tagPathName, 
                get_string("tagPathName", dest->tagPathName, m_tagPathName, RPI_MAX_DATACONTAINER_NAME)
            );
     
            // Components
            if (json_is_array(components)) {
                json_array_foreach(components, component_index, sub_element) {
                    c_tagPathName = json_object_get(sub_element, "tagPathName");
                    settings = json_object_get(sub_element, "settings");
                    tags = json_object_get(sub_element, "tags");
                    dest->components_count++;

                    // Set Component @ component_index
                    DataComponent *c_dest = &dest->components[component_index];
                    SAFE_STRCPY(c_dest->tagPathName, 
                        get_string("tagPathName", c_dest->tagPathName, c_tagPathName, RPI_MAX_DATACOMPONENT_NAME)
                    );

                    // Component settings
                    if (json_is_array(settings)) {
                        json_array_foreach(settings, nv_index, nv_element) {
                            s_name = json_object_get(nv_element, "name");
                            s_value = json_object_get(nv_element, "value");
                            // Add/set to settings pointer list
                            if (json_is_string(s_name) && json_is_string(s_value)) {
                                namevalue_add(
                                    &rpiNode.data[module_index].components[component_index].settings,
                                    json_string_value(s_name), json_string_value(s_value)
                                );
                                //printf("--%s\n", rpiNode.data[module_index].components[component_index].settings->name);
                            }
                        }
                    }

                    // Tags
                    if (json_is_array(tags)) {
                        json_array_foreach(tags, tag_index, t_element) {
                            tag_id = json_object_get(t_element, "id");
                            tag_type = json_object_get(t_element, "type");
                            tag_description = json_object_get(t_element, "description");
                            DataTag *dt = &c_dest->tags[tag_index];
                            SAFE_STRCPY(dt->id,
                                get_string("id", dt->id, tag_id, RPI_MAX_DATATAG_ID)
                            );
                            SAFE_STRCPY(dt->type,
                                get_string("type", dt->type, tag_type, RPI_MAX_META_TYPE)
                            );
                            SAFE_STRCPY(dt->description,
                                get_string("description", dt->description, tag_description, RPI_MAX_META_DESC)
                            );
                            c_dest->tags_count++;
                        }
                    }


                }
            }
        }
    }
*/
}