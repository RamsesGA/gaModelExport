#ifndef __GAEXPORT2_H
#define __GAEXPORT2_H

#include <maya/MDagPath.h>
#include <maya/MFnPlugin.h>

#include <iosfwd>
#include <sstream>

#include "gaExporter.h"

#define	PLUGIN_GA_ENGINE "gaModels"
#define GA_FORMAT "gam"

using std::ofstream;
using std::string;
using std::ostream;

class gaExporter2 : public gaExporter {

public:
  /***************************************************************************/
  /**
  * Constructor and destructor.
  */
  /***************************************************************************/
  gaExporter2() = default;

  ~gaExporter2() = default;

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/

  static void* 
  creator();

  MString
  defaultExtension() const override;

private:
  gaWriter*
  createPolyWriter(const MDagPath dagPath, MStatus& status) override;

  void
  writeHeader(ostream& os) override;

  MString
  filter() const override;

  MFileKind 
  identifyFile(const MFileObject& file,
               const char* buffer,
               short size) const override;

  static MTypeId m_id;
};

#endif /*__POLYEXPORTER_H*/