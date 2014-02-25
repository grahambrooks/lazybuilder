#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <boost/regex.hpp>


class filters {
  typedef std::function<bool (const char *path, FSEventStreamEventFlags flags)> filter;
public:
  filter hidden_file_filter  = [] (const char *path, FSEventStreamEventFlags flags) {
    boost::regex e(".*/\\..*");
    
    return boost::regex_match(path, e);
  };

  filter file_action = [] (const char *path, FSEventStreamEventFlags flags) {
    return ((kFSEventStreamEventFlagItemCreated |
	     kFSEventStreamEventFlagItemRemoved |
	     kFSEventStreamEventFlagItemRenamed |
	     kFSEventStreamEventFlagItemModified) & flags) == 0;
  };
    
  filter no_symbolic_links = [] (const char *path, FSEventStreamEventFlags flags) {
    return (kFSEventStreamEventFlagItemIsSymlink & flags) != 0;
  };

  bool should_ignore(const char* path, FSEventStreamEventFlags flags) {
    return hidden_file_filter(path, flags) ||
      no_symbolic_links(path, flags) ||
      file_action(path, flags);
  }
};

