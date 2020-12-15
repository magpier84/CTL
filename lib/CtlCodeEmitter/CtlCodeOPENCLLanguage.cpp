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

#include "CtlCodeOPENCLLanguage.h"

#include <CtlSimdInterpreter.h>
#include <CtlSimdType.h>
#include <CtlSimdAddr.h>

namespace Ctl
{

OPENCLGenerator::OPENCLGenerator(SimdInterpreter& simdInterpreter)
		: CPPGenerator( false )
		, simdInterpreter( simdInterpreter )
{
}


////////////////////////////////////////


OPENCLGenerator::~OPENCLGenerator(void ) = default;

bool OPENCLGenerator::precalculate(const std::string& name, const ExprNodeVector& args)
{
    try {
        if (auto symbol = simdInterpreter.getSymbol(name)) {
            auto addr = symbol->addr();
            if (addr.is_subclass<SimdDataAddr>()) {
                const auto& dataAddr = addr.cast<SimdDataAddr>();
                const auto& dataType = symbol->type();
                const auto& reg = *dataAddr->reg();
                if (dataType.is_subclass<StructType>()) {
                    auto structType = dataType.cast<StructType>();
                    curStream() << " = { ";
                    int index = 0;
                    bool isFirst = true;
                    for (const auto& member : structType->members()) {
                        if (!isFirst) {
                            curStream() << ", ";
                        } else {
                            isFirst = false;
                        }
                        const auto& memberType = member.type;
                        if (memberType.is_subclass<ArrayType>()) {
                            const auto& arrayType = memberType.cast<ArrayType>();
                            curStream() << "{ ";
                            for (auto i = 0u; i < arrayType->size(); ++i) {
                                if (i > 0) {
                                    curStream() << ", ";
                                }
                                curStream() << *(reinterpret_cast<const float*>(reg[0]) + index);
                                ++index;
                            }
                            curStream() << " }";
                        }
                    }
                    curStream() << " };";

                    return true;
                }
                else if (dataType.is_subclass<ArrayType>()) {
                    const auto& arrayInfo = collapseArray(dataType.cast<ArrayType>());
                    const auto& arrayType = dataType.cast<ArrayType>();
                    curStream() << " = { ";
                    if (!arrayInfo.isCore) {
                        curStream() << "{ ";
                    }
                    int index = 0;
                    bool isFirst = true;
                    for (auto i = 0u; i < arrayType->size(); ++i) {
                        if (!isFirst) {
                            curStream() << ", ";
                        } else {
                            isFirst = false;
                        }
                        const auto& elementType = arrayType->elementType();
                        if (elementType.is_subclass<ArrayType>()) {
                            const auto& subArrayType = elementType.cast<ArrayType>();
                            curStream() << "{ ";
                            for (auto j = 0u; j < subArrayType->size(); ++j) {
                                if (j > 0) {
                                    curStream() << ", ";
                                }
                                curStream() << *(reinterpret_cast<const float*>(reg[0]) + index);
                                ++index;
                            }
                            curStream() << " }";
                        }
                        else if (elementType.is_subclass<FloatType>()) {
                            curStream() << *(reinterpret_cast<const float*>(reg[0]) + index);
                            ++index;
                        }
                    }
                    if (!arrayInfo.isCore) {
                        curStream() << " }";
                    }
                    curStream() << " };";

                    return true;
                }
                else if (dataType.is_subclass<FloatType>()) {
                    curStream() << " = " << *(reinterpret_cast<const float*>(reg[0])) << ";";

                    return true;
                }
            }
        }
//        if (auto call = simdInterpreter.newFunctionCall("ctl" + name)) {
//            for (const auto& arg : args) {
//                if (arg.is_subclass<CodeNameNode>()) {
//                    auto name = arg.cast<CodeNameNode>();
////                    name->info->module()->
//                }
//            }
//
//            call->callFunction(1);
//            const auto res = call->returnValue();
//            if (res->type().is_subclass<SimdArrayType>()) {
//                const auto array = res->type().cast<SimdArrayType>();
//                SizeVector sizes, offsets;
//                array->coreSizes(0, sizes, offsets);
//                for (auto i = 0u; i < sizes.size(); ++i) {
//                    const auto offset = offsets[i];
//                    const auto* data = (float*)(res->data() + offset);
//                    std::cout << (*data) << std::endl;
//                }
//            }
//        }
    }
    catch (...)
    {
        return false;
    }

    return false;
}

const std::string &OPENCLGenerator::getFunctionPrefix() const
{
    static std::string kPrefix = "";
    return kPrefix;
}

const std::string &OPENCLGenerator::getInlineKeyword() const
{
    static std::string kInline = "inline";
    return kInline;
}

const std::string &OPENCLGenerator::getConstLiteral( bool isGlobal ) const
{
    static std::string kGlobalConst = "__constant";
    static std::string kLocalConst = "const";
    return isGlobal ? kGlobalConst : kLocalConst;
}

std::string OPENCLGenerator::constructNamespaceTag(const std::string &modName )
{
    return modName + "_";
}

void OPENCLGenerator::startCast(const char *type )
{
    curStream() << "(" << type << ")(";
}

void OPENCLGenerator::swizzling(int count )
{
    if (count == 3) {
        curStream() << ".xyz";
    } else {
        assert(false);
    }
}

std::string OPENCLGenerator::getPrecisionFunctionSuffix() const
{
    return "";
}

void OPENCLGenerator::initStdLibrary( void )
{
    CPPGenerator::initStdLibrary();
    myStdNames["HALF_POS_INF"] = "0x7c00";
    myStdNames["HALF_NEG_INF"] = "0xfc00";
    myStdNames["HALF_NAN"] = "0x7c01";
    myStdNames["HALF_MIN"] = "0x1.0p-14f";
    myStdNames["HALF_MAX"] = "0x1.ffcp15f";
}

void OPENCLGenerator::defineStandardTypes(std::map<StdType, TypeDefinition>& types,
                                          const std::string& funcPreffix,
                                          const std::string& precSuffix)
{
    types[StdType::Float] = TypeDefinition("float",  "#define ctl_number_t float");

    types[StdType::Vec2f] = TypeDefinition("float2", "typedef float2 ctl_vec2f_t;", "(float2)", "");
    types[StdType::Vec3f] = TypeDefinition("float3", "typedef float3 ctl_vec3f_t;\n#define make_vec3f (float3)", "(float3)", "");
    types[StdType::Vec4f] = TypeDefinition("float4", "typedef float4 ctl_vec4f_t;", "(float4)", "");
    types[StdType::Vec8f] = TypeDefinition("float8", "typedef float8 ctl_vec8f_t;", "(float8)", "");

    types[StdType::Vec2i] = TypeDefinition("int2", "typedef int2 ctl_vec2i_t;", "(int2)", "");
    types[StdType::Vec3i] = TypeDefinition("int3", "typedef int3 ctl_vec3i_t;", "(int3)", "");
    types[StdType::Vec4i] = TypeDefinition("int4", "typedef int4 ctl_vec4i_t;", "(int4)", "");
    types[StdType::Vec8i] = TypeDefinition("int8", "typedef int8 ctl_vec8i_t;", "(int8)", "");

    types[StdType::Mat3f] = TypeDefinition(
        "ctl_mat33f_t",
        "typedef struct { float vals[3][3]; } ctl_mat33f_t;",
        "make_mat33f",
        funcPreffix + "ctl_mat33f_t make_mat33f(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)\n"
        "{\n"
        "    ctl_mat33f_t r;\n"
        "    r.vals[0][0] = m00;\n"
        "    r.vals[1][0] = m01;\n"
        "    r.vals[2][0] = m02;\n"
        "    r.vals[0][1] = m10;\n"
        "    r.vals[1][1] = m11;\n"
        "    r.vals[2][1] = m12;\n"
        "    r.vals[0][2] = m20;\n"
        "    r.vals[1][2] = m21;\n"
        "    r.vals[2][2] = m22;\n"
        "    return r;\n"
        "}");

    types[StdType::Mat4f] = TypeDefinition(
        "ctl_mat44f_t",
        "typedef struct { float vals[4][4]; } ctl_mat44f_t;",
        funcPreffix +
        "ctl_mat44f_t make_mat44f(float m00, float m01, float m02, float m03,"
                                 "float m10, float m11, float m12, float m13,"
                                 "float m20, float m21, float m22, float m23,"
                                 "float m30, float m31, float m32, float m33 )\n"
        "{\n"
        "    ctl_mat44f_t r;\n"
        "    r.vals[0][0] = m00;\n"
        "    r.vals[0][1] = m01;\n"
        "    r.vals[0][2] = m02;\n"
        "    r.vals[0][3] = m03;\n"
        "    r.vals[1][0] = m10;\n"
        "    r.vals[1][1] = m11;\n"
        "    r.vals[1][2] = m12;\n"
        "    r.vals[1][3] = m13;\n"
        "    r.vals[2][0] = m20;\n"
        "    r.vals[2][1] = m21;\n"
        "    r.vals[2][2] = m22;\n"
        "    r.vals[2][3] = m23;\n"
        "    r.vals[3][0] = m30;\n"
        "    r.vals[3][1] = m31;\n"
        "    r.vals[3][2] = m32;\n"
        "    r.vals[3][3] = m33;\n"
        "    return r;\n"
        "}");

	types[StdType::Chromaticities] = TypeDefinition(
	    "Chromaticities",
	    "typedef struct { float2 red; float2 green; float2 blue; float2 white; } Chromaticities;" );
}

void OPENCLGenerator::getStandardMathBodies(FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix )
{
    CPPGenerator::getStandardMathBodies(d, funcPref, precSuffix);

//    d.push_back(
//        FunctionDefinition(
//            "mult_f_f3",
//            "inline float3 mult_f_f3(const float a, const float3 b) { return a * b; }"));
//
//    d.push_back(
//        FunctionDefinition(
//            "mult_f3_f33",
//            R"(
//inline float3 mult_f3_f33(const float3 a, const ctl_mat33f_t b)
//{
//    float3 rgb;
//    for(int j = 0; j < 3; ++j) {
//        rgb[j] = 0.0f;
//        for(int i = 0; i < 3; ++i) rgb[j] += b.vals[j][i] * a[i];
//    }
//    return rgb;
//})"));
//
//    d.push_back(
//        FunctionDefinition(
//            "mult_f33_f33",
//            R"(
//inline ctl_mat33f_t mult_f33_f33(const ctl_mat33f_t a, const ctl_mat33f_t b)
//{
//    ctl_mat33f_t c;
////    for(int j = 0; j < 3; ++j) {
////        rgb[j] = 0.0f;
////        for(int i = 0; i < 3; ++i) rgb[j] += b[3 * j + i] * a[i];
////    }
//    return c;
//})"));
//
//    d.push_back(
//        FunctionDefinition(
//            "invert_33",
//            R"(
//inline ctl_mat33f_t invert_f33(const ctl_mat33f_t a)
//{
//    ctl_mat33f_t b;
//    return b;
//})"));
//
//    d.push_back(
//        FunctionDefinition(
//            "invert_f44",
//            R"(
//inline ctl_mat44f_t invert_f44(const ctl_mat44f_t a)
//{
//    ctl_mat44f_t b;
//    return b;
//})"));
//
//    d.push_back(
//        FunctionDefinition(
//            "pow10",
//            "inline float pow10( const float x ) { return pow(10.0f, x); }"));
}

void OPENCLGenerator::getStandardHalfBodies(FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix )
{

}

void OPENCLGenerator::getStandardPrintBodies(FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix )
{
    d.push_back( FunctionDefinition( "assert", funcPref + "void assert( bool v ) {}" ) );
    d.push_back( FunctionDefinition( "print_float", funcPref + "void print_float( ctl_number_t v ) { printf(\"%d\", v); }" ) );
}

void OPENCLGenerator::getStandardColorBodies(FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix )
{
    CPPGenerator::getStandardColorBodies(d, funcPref, precSuffix);
}

void OPENCLGenerator::getStandardInterpBodies(FuncDeclList &d, const std::string &funcPref, const std::string &precSuffix )
{
    CPPGenerator::getStandardInterpBodies(d, funcPref, precSuffix);
}

} // namespace Ctl


