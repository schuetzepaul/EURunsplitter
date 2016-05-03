/*
 * Author: Paul Schuetze
 *
 * Copied from eudaq
 *
 *
 *
 */



#include "eudaq/FileWriter.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/FileWriter.hh"
#include "eudaq/FileSerializer.hh"
//#include "eudaq/Logger.hh"

#include <string>


namespace eudaq {

  class FileWriterNative : public FileWriter {
  public:
    FileWriterNative(const std::string &);
    virtual void StartRun(unsigned);
    virtual void WriteEvent(const DetectorEvent &);
    virtual uint64_t FileBytes() const;
    virtual ~FileWriterNative();
    
    void SetPattern(const std::string &);

  private:
    FileSerializer *m_ser;
    std::string m_pattern;
  };

  namespace {
    static RegisterFileWriter<FileWriterNative> reg("native");
  }

  FileWriterNative::FileWriterNative(const std::string & /*param*/) : m_ser(0) {
    // EUDAQ_DEBUG("Constructing FileWriterNative(" + to_string(param) + ")");
  }

  void FileWriterNative::StartRun(unsigned runnumber) {
    delete m_ser;
    
    m_ser = new FileSerializer(
			       FileNamer(m_pattern).Set('X', ".raw").Set('R', runnumber));
  }

  void FileWriterNative::WriteEvent(const DetectorEvent &ev) {
    if (!m_ser)
      EUDAQ_THROW("FileWriterNative: Attempt to write unopened file");
    m_ser->write(ev);
    m_ser->Flush();
  }
  
  void FileWriterNative::SetPattern(const std::string &filename) {
    m_pattern = filename;
  }

  FileWriterNative::~FileWriterNative() { delete m_ser; }

  uint64_t FileWriterNative::FileBytes() const {
    return m_ser ? m_ser->FileBytes() : 0;
  }
}

