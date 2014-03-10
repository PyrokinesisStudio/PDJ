#include "patent_office.h"

namespace pdp {

PatentOffice* getPatentOffice() {
	static PatentOffice patentOffice;
	return &patentOffice;
}


}
