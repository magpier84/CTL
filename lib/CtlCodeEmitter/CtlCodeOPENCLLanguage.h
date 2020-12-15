///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2013 Academy of Motion Picture Arts and Sciences
// ("A.M.P.A.S."). Portions contributed by others as indicated.
// All rights reserved.
// 
// A world-wide, royalty-free, non-exclusive right to distribute, copy,
// modify, create derivatives, and use, in source and binary forms, is
// hereby granted, subject to acceptance of this license. Performance of
// any of the aforementioned acts indicates acceptance to be bound by the
// following terms and conditions:
// 
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the Disclaimer of Warranty.
// 
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the Disclaimer of Warranty
//     in the documentation and/or other materials provided with the
//     distribution.
// 
//   * Nothing in this license shall be deemed to grant any rights to
//     trademarks, copyrights, patents, trade secrets or any other
//     intellectual property of A.M.P.A.S. or any contributors, except
//     as expressly stated herein, and neither the name of A.M.P.A.S.
//     nor of any other contributors to this software, may be used to
//     endorse or promote products derived from this software without
//     specific prior written permission of A.M.P.A.S. or contributor,
//     as appropriate.
// 
// This license shall be governed by the laws of the State of California,
// and subject to the jurisdiction of the courts therein.
// 
// Disclaimer of Warranty: THIS SOFTWARE IS PROVIDED BY A.M.P.A.S. AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT ARE DISCLAIMED. IN NO
// EVENT SHALL A.M.P.A.S., ANY CONTRIBUTORS OR DISTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_CTL_CODE_OPENCL_LANGUAGE_H
#define INCLUDED_CTL_CODE_OPENCL_LANGUAGE_H

#include "CtlCodeCPPLanguage.h"

namespace Ctl
{
class SimdInterpreter;

// largely the same, init code is different,
// function keywords added.
// Boiler plate for launching kernels different
class OPENCLGenerator : public CPPGenerator
{
public:
	explicit OPENCLGenerator(SimdInterpreter& simdInterpreter);
	virtual ~OPENCLGenerator(void );

    bool supportsReferences( void ) const { return false; }
    bool supportsPointers( void ) const { return true; }
    bool supportsStructOperators( void ) const { return false; }
    bool supportsStructConstructors( void ) const { return false; }
    bool supportsArrayInitializers( void ) const { return true; }
    bool usesFunctionInitializers( void ) const { return false; }
    bool supportsNamespaces( void ) const { return false; }
    bool supportsHalfType( void ) const { return false; }
    bool supportsPrint() const { return false; }

    void initStdLibrary( void );
    std::string stdLibraryAndSetup( void ) override { return ""; }

    bool precalculate(const std::string& name, const ExprNodeVector& args) override;

protected:
    const std::string &getFunctionPrefix( void ) const override;
    const std::string &getInlineKeyword( void ) const override;
    const std::string &getConstLiteral( bool isGlobal ) const override;
    std::string constructNamespaceTag( const std::string &modName ) override;

    void startCast( const char *type ) override;
    void swizzling( int count ) override;

    std::string getPrecisionFunctionSuffix( void ) const override;

    void defineStandardTypes( std::map<StdType, TypeDefinition> &types, const std::string &funcPref, const std::string & precSuffix ) override;
    virtual void getStandardMathBodies( FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix );
    virtual void getStandardHalfBodies( FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix );
    virtual void getStandardPrintBodies( FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix );
    virtual void getStandardColorBodies( FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix );
    virtual void getStandardInterpBodies( FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix );

private:
    SimdInterpreter& simdInterpreter;
};

} // namespace Ctl

#endif // INCLUDED_CTL_CODE_OPENCL_LANGUAGE_H
