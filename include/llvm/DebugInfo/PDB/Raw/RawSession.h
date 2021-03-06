//===- RawSession.h - Native implementation of IPDBSession ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_PDB_RAW_RAWSESSION_H
#define LLVM_DEBUGINFO_PDB_RAW_RAWSESSION_H

#include "llvm/DebugInfo/PDB/IPDBSession.h"

namespace llvm {
class PDBFile;
class StringRef;

class RawSession : public IPDBSession {
public:
  explicit RawSession(std::unique_ptr<PDBFile> PdbFile);
  ~RawSession() override;

  static PDB_ErrorCode createFromPdb(StringRef Path,
                                     std::unique_ptr<IPDBSession> &Session);
  static PDB_ErrorCode createFromExe(StringRef Path,
                                     std::unique_ptr<IPDBSession> &Session);

  uint64_t getLoadAddress() const override;
  void setLoadAddress(uint64_t Address) override;
  std::unique_ptr<PDBSymbolExe> getGlobalScope() const override;
  std::unique_ptr<PDBSymbol> getSymbolById(uint32_t SymbolId) const override;

  std::unique_ptr<PDBSymbol>
  findSymbolByAddress(uint64_t Address, PDB_SymType Type) const override;

  std::unique_ptr<IPDBEnumLineNumbers>
  findLineNumbers(const PDBSymbolCompiland &Compiland,
                  const IPDBSourceFile &File) const override;
  std::unique_ptr<IPDBEnumLineNumbers>
  findLineNumbersByAddress(uint64_t Address, uint32_t Length) const override;

  std::unique_ptr<IPDBEnumSourceFiles>
  findSourceFiles(const PDBSymbolCompiland *Compiland, llvm::StringRef Pattern,
                  PDB_NameSearchFlags Flags) const override;
  std::unique_ptr<IPDBSourceFile>
  findOneSourceFile(const PDBSymbolCompiland *Compiland,
                    llvm::StringRef Pattern,
                    PDB_NameSearchFlags Flags) const override;
  std::unique_ptr<IPDBEnumChildren<PDBSymbolCompiland>>
  findCompilandsForSourceFile(llvm::StringRef Pattern,
                              PDB_NameSearchFlags Flags) const override;
  std::unique_ptr<PDBSymbolCompiland>
  findOneCompilandForSourceFile(llvm::StringRef Pattern,
                                PDB_NameSearchFlags Flags) const override;
  std::unique_ptr<IPDBEnumSourceFiles> getAllSourceFiles() const override;
  std::unique_ptr<IPDBEnumSourceFiles> getSourceFilesForCompiland(
      const PDBSymbolCompiland &Compiland) const override;
  std::unique_ptr<IPDBSourceFile>
  getSourceFileById(uint32_t FileId) const override;

  std::unique_ptr<IPDBEnumDataStreams> getDebugStreams() const override;

  PDBFile &getPDBFile() { return *Pdb; }
  const PDBFile &getPDBFile() const { return *Pdb; }

private:
  std::unique_ptr<PDBFile> Pdb;
};
}

#endif
