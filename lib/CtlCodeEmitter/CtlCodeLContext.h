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


#ifndef INCLUDED_CTL_CODE_L_CONTEXT_H
#define INCLUDED_CTL_CODE_L_CONTEXT_H

//-----------------------------------------------------------------------------
//
//	class CodeLContext
//
//-----------------------------------------------------------------------------

#include <CtlLContext.h>
#include <CtlSymbolTable.h>
#include <list>
#include <vector>

namespace Ctl {

class CodeReg;
class CodeInst;
class CodeCallInst;
class CodeModule;
class LanguageGenerator;

class Type;
typedef RcPtr <Type> TypePtr;

class CodeLContext: public LContext
{
public:
    CodeLContext( LanguageGenerator &gen,
				  Module *module,
				  SymbolTable &symtab );

	LanguageGenerator &generator( void ) { return myLanguageGen; }

	const std::vector< std::pair< std::string, MemberVector > > &structs( void ) const { return myStructDefines; }

	virtual void	newStackFrame ();
    virtual AddrPtr	parameterAddr (const DataTypePtr &parameterType);
    virtual AddrPtr	returnValueAddr (const DataTypePtr &returnType);
    virtual AddrPtr	autoVariableAddr (const DataTypePtr &variableType);
    virtual std::vector<DataTypePtr> &locals( void );

	//-----------------------------------------------
    // Factory for syntax tree nodes and type objects
    //-----------------------------------------------
    
    virtual ModuleNodePtr	newModuleNode( int lineNumber,
										   const StatementNodePtr &constants,
										   const FunctionNodePtr &functions ) const;

    virtual FunctionNodePtr	newFunctionNode( int lineNumber,
											 const std::string &name,
											 const SymbolInfoPtr &info,
											 const StatementNodePtr &body ) const;

    virtual VariableNodePtr	newVariableNode( int lineNumber,
											 const std::string &name,
											 const SymbolInfoPtr &info,
											 const ExprNodePtr &initialValue,
											 bool assignInitialValue ) const;

    virtual AssignmentNodePtr	newAssignmentNode( int lineNumber,
												   const ExprNodePtr &lhs,
												   const ExprNodePtr &rhs ) const;

    virtual ExprStatementNodePtr newExprStatementNode( int lineNumber,
													   const ExprNodePtr &expr ) const;

    virtual IfNodePtr		newIfNode( int lineNumber,
									   const ExprNodePtr &condition,
									   const StatementNodePtr &truePath,
									   const StatementNodePtr &falsePath ) const;

    virtual ReturnNodePtr	newReturnNode( int lineNumber,
										   const SymbolInfoPtr &info,
										   const ExprNodePtr &returnedValue ) const;

    virtual WhileNodePtr	newWhileNode( int lineNumber,
										  const ExprNodePtr &condition,
										  const StatementNodePtr &loopBody ) const;

    virtual ForNodePtr      newForNode( int lineNumber,
                                        const ExprNodePtr &cond,
                                        const StatementNodePtr &init,
                                        const StatementNodePtr &update,
                                        const StatementNodePtr &loopBody ) const;

    virtual BinaryOpNodePtr	newBinaryOpNode( int lineNumber,
											 Token op,
											 const ExprNodePtr &leftOperand,
											 const ExprNodePtr &rightOperand ) const;

    virtual UnaryOpNodePtr	newUnaryOpNode( int lineNumber,
											Token op,
											const ExprNodePtr &operand) const;

    virtual ArrayIndexNodePtr	newArrayIndexNode( int lineNumber,
												   const ExprNodePtr &array,
												   const ExprNodePtr &index ) const;

    virtual MemberNodePtr	newMemberNode( int lineNumber,
										   const ExprNodePtr &obj,
										   const std::string &member ) const;

    virtual SizeNodePtr	        newSizeNode( int lineNumber,
											 const ExprNodePtr &obj ) const;

    virtual NameNodePtr		newNameNode( int lineNumber,
										 const std::string &name,
										 const SymbolInfoPtr &info ) const;

    virtual BoolLiteralNodePtr	newBoolLiteralNode( int lineNumber, bool value ) const;

    virtual IntLiteralNodePtr	newIntLiteralNode( int lineNumber, int value ) const;

    virtual UIntLiteralNodePtr	newUIntLiteralNode( int lineNumber, unsigned value ) const;

    virtual HalfLiteralNodePtr	newHalfLiteralNode( int lineNumber, half value ) const;

    virtual FloatLiteralNodePtr	newFloatLiteralNode( int lineNumber, float value ) const;

    virtual StringLiteralNodePtr newStringLiteralNode( int lineNumber,
													   const std::string &value ) const;

    virtual CallNodePtr		newCallNode( int lineNumber,
										 const NameNodePtr &function,
										 const ExprNodeVector &arguments ) const;

    virtual ValueNodePtr	newValueNode( int lineNumber,
										  const ExprNodeVector &elements ) const;

    virtual VoidTypePtr		newVoidType( void ) const;

    virtual BoolTypePtr		newBoolType( void ) const;

    virtual IntTypePtr		newIntType( void ) const;

    virtual UIntTypePtr		newUIntType( void ) const;

    virtual HalfTypePtr		newHalfType( void ) const;
    
    virtual FloatTypePtr	newFloatType( void ) const;
    
    virtual StringTypePtr	newStringType( void ) const;


    virtual ArrayTypePtr	newArrayType( const DataTypePtr &baseType,
										  int size,
										  ArrayTypeUsage usage = NON_PARAMETER );

    virtual StructTypePtr	newStructType( const std::string &name,
										   const MemberVector &members ) const;

    virtual FunctionTypePtr	newFunctionType( const DataTypePtr &returnType,
											 bool returnVarying,
											 const ParamVector &parameters ) const;

    void setLoop(bool isLoop) { _isLoop = isLoop; }
    bool isLoop() const { return _isLoop; }

private:
    int			_nextParameterAddr;

    bool _isLoop = false;
    std::vector<DataTypePtr> _locals;
	mutable std::vector< std::pair<std::string, MemberVector> > myStructDefines;

	LanguageGenerator &myLanguageGen;
};


} // namespace Ctl

#endif
