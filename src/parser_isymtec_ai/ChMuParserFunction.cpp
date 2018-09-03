
#include "parser_isymtec_ai/ChMuParserFunction.h"
#include "physics/ChObject.h"
#include <memory>
#include "ChIsymtecAiUtils.h"
#include "muparser/muParser.h"

using namespace chrono;

namespace {
	std::shared_ptr<mu::Parser> createParser()
	{
		auto output = std::make_shared<mu::Parser>();
		output->DefineConst("pi", CH_C_PI);
		output->DefineConst("PI", CH_C_PI);
		output->DefineConst("e", CH_C_E);
		output->DefineConst("E", CH_C_E);
		return output;
	}
}


ChMuParserFunction::ChMuParserFunction()
{
	m_ParserValue = createParser();
	m_ParserValueDif = createParser();
	m_ParserValueDifDif = createParser();
}

double ChMuParserFunction::Get_y(double x) const
{
	m_InputValue = x;
	double output = m_ParserValue->Eval();
	return output;
}

double ChMuParserFunction::Get_y_dx(double x) const
{
	double output;
	if (m_CountDerivatives > 0) {
		m_InputValue = x;
		output = m_ParserValueDif->Eval();
	}
	else {
		output = chrono::ChFunction::Get_y_dx(x);
	}
	return output;
}

double ChMuParserFunction::Get_y_dxdx(double x) const
{
	double output;
	if (m_CountDerivatives > 1) {
		m_InputValue = x;
		output = m_ParserValueDifDif->Eval();
	}
	else {
		output = chrono::ChFunction::Get_y_dxdx(x);
	}
	return output;
}



ChMuParserFunction* ChMuParserFunction::Clone() const
{
	throw chrono::ChException("Clone method of ChMuParserFunction is not implemented yet" );
	//auto* output = new ChMuParserFunction();
	//auto& newParser = std::shared_ptr<mu::Parser>(new mu::Parser(*m_Parser));
	//output->setParser(newParser);
	//return output;
}

void ChMuParserFunction::DefineInputVariable(std::string inputVar)
{
	m_InputVariableName = inputVar;
	m_ParserValue->DefineVar(m_InputVariableName, &m_InputValue);
	m_ParserValueDif->DefineVar(m_InputVariableName, &m_InputValue);
	m_ParserValueDifDif->DefineVar(m_InputVariableName, &m_InputValue);
}


void ChMuParserFunction::SetExpresson(const std::string &expr)
{
	m_ParserValue->SetExpr(expr);
}


void ChMuParserFunction::SetExpressionDif(const std::string & expr) {
	m_ParserValueDif->SetExpr(expr);
}

void ChMuParserFunction::SetExpressionDifDif(const std::string & expr) {
	m_ParserValueDifDif->SetExpr(expr);
}