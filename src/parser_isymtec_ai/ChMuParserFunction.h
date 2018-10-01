#ifndef CH_MU_PARSER_FUNCTION
#define CH_MU_PARSER_FUNCTION

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include <map>
#include <vector>
#include "motion_functions/ChFunction_Base.h"
#include "chrono/physics/ChObject.h"

namespace mu {
	class Parser;
}
namespace chrono {
	class ChSystem;
}

/// @addtogroup parser_isymtec_ai
/// Class, creating function from string expression, using muparser library

class ChApiIsymtecAI ChMuParserFunction : public chrono::ChFunction, public chrono::ChObj {
public:
	ChMuParserFunction();;

	virtual double Get_y(double x) const override;
	virtual double Get_y_dx(double x) const override;
	virtual double Get_y_dxdx(double x) const override;

	virtual ChMuParserFunction* Clone() const override;

	///name of the input variable, used as input parameter in function Get_y
	void DefineInputVariable(std::string inputVar);

	void SetExpresson(const std::string &expr);

	void SetExpressionDif(const std::string &expr);

	void SetExpressionDifDif(const std::string &expr);

	void SetCountDerivatives(unsigned value) { m_CountDerivatives = value; }

private:
	unsigned m_CountDerivatives{ 0 };
	std::string m_InputVariableName;
	void DefineConstants();

	mutable double m_InputValue;

	std::shared_ptr<mu::Parser> m_ParserValue;
	std::shared_ptr<mu::Parser> m_ParserValueDif;
	std::shared_ptr<mu::Parser> m_ParserValueDifDif;
};

#endif
