#include <boost/assign.hpp>

#include "metadata.hpp"

const std::string Drill::meta::BasicMetadata::s_connectorName(DRILL_CONNECTOR_NAME);
const std::string Drill::meta::BasicMetadata::s_connectorVersion(DRILL_VERSION_STRING);
const std::string Drill::meta::BasicMetadata::s_serverName(DRILL_NAME);
const std::string Drill::meta::BasicMetadata::s_serverVersion("0.0.0");

const std::string Drill::meta::BasicMetadata::s_catalogSeparator(".");
const std::string Drill::meta::BasicMetadata::s_catalogTerm("catalog");
const std::string Drill::meta::BasicMetadata::s_identifierQuoteString("`");
const std::vector<std::string> Drill::meta::BasicMetadata::s_sqlKeywords = boost::assign::list_of
		("ABS")("ALLOW")("ARRAY")("ASENSITIVE")("ASYMMETRIC")("ATOMIC")("BIGINT")("BINARY")("BLOB")
		("BOOLEAN")("CALL")("CALLED")("CARDINALITY")("CEIL")("CEILING")("CLOB")("COLLECT")("CONDITION")
		("CORR")("COVAR_POP")("COVAR_SAMP")("CUBE")("CUME_DIST")("CURRENT_CATALOG")
		("CURRENT_DEFAULT_TRANSFORM_GROUP")("CURRENT_PATH")("CURRENT_ROLE")("CURRENT_SCHEMA")
		("CURRENT_TRANSFORM_GROUP_FOR_TYPE")("CYCLE")("DATABASE")("DATABASES")("DENSE_RANK")("DEREF")
		("DETERMINISTIC")("DISALLOW")("DYNAMIC")("EACH")("ELEMENT")("EVERY")("EXP")("EXPLAIN")
		("EXTEND")("FILES")("FILTER")("FIRST_VALUE")("FLOOR")("FREE")("FUNCTION")("FUSION")("GROUPING")
		("HOLD")("IF")("IMPORT")("INOUT")("INTERSECTION")("LARGE")("LAST_VALUE")("LATERAL")("LIMIT")("LN")
		("LOCALTIME")("LOCALTIMESTAMP")("MEMBER")("MERGE")("METADATA")("METHOD")("MOD")("MODIFIES")
		("MULTISET")("NCLOB")("NEW")("NONE")("NORMALIZE")("OFFSET")("OLD")("OUT")("OVER")("OVERLAY")
		("PARAMETER")("PARTITION")("PERCENTILE_CONT")("PERCENTILE_DISC")("PERCENT_RANK")("POWER")
		("RANGE")("RANK")("READS")("RECURSIVE")("REF")("REFERENCING")("REFRESH")("REGR_AVGX")("REGR_AVGY")
		("REGR_COUNT")("REGR_INTERCEPT")("REGR_R2")("REGR_SLOPE")("REGR_SXX")("REGR_SXY")("REGR_SYY")
		("RELEASE")("REPLACE")("RESET")("RESULT")("RETURN")("RETURNS")("ROLLUP")("ROW")("ROW_NUMBER")
		("SAVEPOINT")("SCHEMAS")("SCOPE")("SEARCH")("SENSITIVE")("SHOW")("SIMILAR")("SPECIFIC")("SPECIFICTYPE")
		("SQLEXCEPTION")("SQLWARNING")("SQRT")("START")("STATIC")("STDDEV_POP")("STDDEV_SAMP")("STREAM")
		("SUBMULTISET")("SYMMETRIC")("SYSTEM")("TABLES")("TABLESAMPLE")("TINYINT")("TREAT")("TRIGGER")
		("UESCAPE")("UNNEST")("UPSERT")("USE")("VARBINARY")("VAR_POP")("VAR_SAMP")("WIDTH_BUCKET")
		("WINDOW")("WITHIN")("WITHOUT");

const std::vector<std::string> Drill::meta::BasicMetadata::s_numericFunctions = boost::assign::list_of
		("ABS")("ACOS")("ASIN")("ATAN")("ATAN2")("CEILING")("COS")("COT")
		("DEGREES")("EXP")("FLOOR")("LOG")("LOG10")("MOD")("PI")
		("POWER")("RADIANS")("RAND")("ROUND")("SIGN")("SIN")("SQRT")
		("TAN")("TRUNCATE");
const std::string Drill::meta::BasicMetadata::s_schemaTerm("schema");
const std::string Drill::meta::BasicMetadata::s_searchEscapeString("\\");
const std::string Drill::meta::BasicMetadata::s_specialCharacters;

const std::vector<std::string> Drill::meta::BasicMetadata::s_stringFunctions = boost::assign::list_of
		("ASCII")("CHAR")("CONCAT")("DIFFERENCE")("INSERT")("LCASE")
		("LEFT")("LENGTH")("LOCATE")("LTRIM")("REPEAT")("REPLACE")
		("RIGHT")("RTRIM")("SOUNDEX")("SPACE")("SUBSTRING")("UCASE");

const std::vector<std::string> Drill::meta::BasicMetadata::s_systemFunctions = boost::assign::list_of
		("DATABASE")("IFNULL")("USER");

const std::string Drill::meta::BasicMetadata::s_tableTerm("table");

const std::vector<std::string> Drill::meta::BasicMetadata::s_dateTimeFunctions = boost::assign::list_of
		("CURDATE")("CURTIME")("DAYNAME")("DAYOFMONTH")("DAYOFWEEK")
		("DAYOFYEAR")("HOUR")("MINUTE")("MONTH")("MONTHNAME")("NOW")
		("QUARTER")("SECOND")("TIMESTAMPADD")("TIMESTAMPDIFF")("WEEK")("YEAR");
