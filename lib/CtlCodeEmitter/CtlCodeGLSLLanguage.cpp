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

#include "CtlCodeGLSLLanguage.h"

namespace Ctl
{

GLSLGenerator::GLSLGenerator(void )
		: CPPGenerator( false )
{
}


////////////////////////////////////////


GLSLGenerator::~GLSLGenerator(void )
{
}

const std::string &GLSLGenerator::getFunctionPrefix(void ) const
{
    static std::string kPrefix = "";
    return kPrefix;
}

const std::string &GLSLGenerator::getInlineKeyword(void ) const
{
    static std::string kInline = "";
    return kInline;
}

std::string GLSLGenerator::constructNamespaceTag(const std::string &modName )
{
    return modName + "_";
}

void GLSLGenerator::startCast(const char *type )
{
    curStream() << type << "( ";
}

void GLSLGenerator::swizzling(int count )
{
    if (count == 3) {
        curStream() << ".xyz";
    } else {
        assert(false);
    }
}

std::string GLSLGenerator::getPrecisionFunctionSuffix(void ) const
{
    return "";
}

void GLSLGenerator::defineStandardTypes(std::map<StdType, TypeDefinition>& types, const std::string& funcPref,
                                        const std::string& precSuffix)
{
    types[StdType::Float] = TypeDefinition("float", "", "");
    types[StdType::Vec3f] = TypeDefinition("vec3", "", "");
    types[StdType::Vec4f] = TypeDefinition("vec4", "", "");
    types[StdType::Mat3f] = TypeDefinition("mat3", "", "");
    types[StdType::Mat4f] = TypeDefinition("mat4", "", "");
}

void GLSLGenerator::getStandardMathBodies(FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix )
{
    d.push_back(
        FunctionDefinition(
            "mult_f_f3",
            "vec3 mult_f_f3( const float a, const vec3 b ) { return a * b; }" ) );

    d.push_back(
        FunctionDefinition(
            "mult_f3_f33",
            "vec3 mult_f3_f33( const vec3 a, const mat3 b ) { return b * a; }" ) );

    d.push_back(
        FunctionDefinition(
            "pow10",
            "float pow10( const float x ) { return pow(10.0, x); }" ) );

    d.push_back(
        FunctionDefinition(
            "log10",
            "float log10( const float x ) { return (1.0 / log(10)) * log(x); }" ) );
}

void GLSLGenerator::getStandardHalfBodies(FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix )
{

}

void GLSLGenerator::getStandardPrintBodies(FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix )
{
//    d.push_back( FunctionDefinition( "assert", funcPref + "void assert( bool v ) {}"));
//    d.push_back( FunctionDefinition( "print_bool", funcPref + "void print_bool( bool v ) {}"));
//    d.push_back( FunctionDefinition( "print_int", funcPref + "void print_int( int v ) {}"));
//    d.push_back( FunctionDefinition( "print_unsigned_int", funcPref + "void print_unsigned_int( unsigned int ) {}"));
//    d.push_back( FunctionDefinition( "print_half", funcPref + "void print_half( const half& ) {}"));
//    d.push_back( FunctionDefinition( "print_float", funcPref + "void print_float( float ) {}"));
//    d.push_back( FunctionDefinition( "print_string", funcPref + "void print_string( const char* ) {}"));
}

void GLSLGenerator::getStandardColorBodies(FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix )
{

}

void GLSLGenerator::getStandardInterpBodies(FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix )
{

}

} // namespace Ctl


