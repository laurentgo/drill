/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.drill.jdbc;

import static org.hamcrest.CoreMatchers.anyOf;
import static org.hamcrest.CoreMatchers.containsString;
import static org.hamcrest.CoreMatchers.equalTo;
import static org.junit.Assert.assertThat;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Random;

import org.apache.drill.categories.JdbcTest;
import org.apache.drill.exec.ExecConstants;
import org.apache.drill.exec.client.DrillClient;
import org.apache.drill.exec.physical.impl.ScreenCreator;
import org.apache.drill.exec.proto.helper.QueryIdHelper;
import org.apache.drill.exec.testing.Controls;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

/**
 * Test for Drill's implementation of Statement's methods (most).
 */
@Category(JdbcTest.class)
public class StatementTest extends JdbcTestBase {

  private static final String SYS_VERSION_SQL = "select * from sys.version";

  private static Connection connection;

  @BeforeClass
  public static void setUpStatement() throws SQLException {
    // (Note: Can't use JdbcTest's connect(...) because JdbcTest closes
    // Connection--and other JDBC objects--on test method failure, but this test
    // class uses some objects across methods.)
    connection = new Driver().connect( "jdbc:drill:zk=local", null );
  }

  @AfterClass
  public static void tearDownStatement() throws SQLException {
    connection.close();
  }


  ////////////////////////////////////////
  // Query timeout methods:

  //////////
  // getQueryTimeout():

  /**
   * Test for reading of default query timeout
   */
  @Test
  public void testGetQueryTimeoutSaysNoTimeout() throws SQLException {
    try(Statement statement = connection.createStatement()) {
      assertThat( statement.getQueryTimeout(), equalTo( 0 ) );
    }
  }

  //////////
  // setQueryTimeout(...):

  /** Tests that setQueryTimeout(...) accepts (redundantly) setting to
   *  no-timeout mode. */
  @Test
  public void testSetQueryTimeoutAcceptsNotimeoutRequest() throws SQLException {
    try(Statement statement = connection.createStatement()) {
      statement.setQueryTimeout( 0 );
    }
  }

  @Test( expected = InvalidParameterSqlException.class )
  public void testSetQueryTimeoutRejectsBadTimeoutValue() throws SQLException {
    try(Statement statement = connection.createStatement()) {
      statement.setQueryTimeout( -2 );
    }
    catch ( InvalidParameterSqlException e ) {
      // Check exception for some mention of parameter name or semantics:
      assertThat( e.getMessage(), anyOf( containsString( "milliseconds" ),
                                         containsString( "timeout" ),
                                         containsString( "Timeout" ) ) );
      throw e;
    }
  }

  /**
   * Test setting a valid timeout
   */
  @Test
  public void testValidSetQueryTimeout() throws SQLException {
    try(Statement statement = connection.createStatement()) {
      // Setting positive value
      int valueToSet = new Random(System.currentTimeMillis()).nextInt(59) + 1;
      statement.setQueryTimeout(valueToSet);
      assertThat( statement.getQueryTimeout(), equalTo( valueToSet ) );
    };
  }

  /**
   * Test setting timeout for a query that actually times out
   */
  @Test ( expected = SqlTimeoutException.class )
  public void testTriggeredQueryTimeout() throws SQLException {
    // Prevent the server to complete the query to trigger a timeout
    final String controls = Controls.newBuilder()
      .addPause(ScreenCreator.class, "send-complete", 0)
      .build();

    try(Statement statement = connection.createStatement()) {
      assertThat(
          statement.execute(String.format(
              "ALTER session SET `%s` = '%s'",
              ExecConstants.DRILLBIT_CONTROL_INJECTIONS,
              controls)),
          equalTo(true));
    }
    String queryId = null;
    try(Statement statement = connection.createStatement()) {
      int timeoutDuration = 3;
      //Setting to a very low value (3sec)
      statement.setQueryTimeout(timeoutDuration);
      ResultSet rs = statement.executeQuery(SYS_VERSION_SQL);
      queryId = ((DrillResultSet) rs).getQueryId();
      //Fetch rows
      while (rs.next()) {
        rs.getBytes(1);
      }
    } catch (SQLException sqlEx) {
      if (sqlEx instanceof SqlTimeoutException) {
        throw (SqlTimeoutException) sqlEx;
      }
    } finally {
      // Do not forget to unpause to avoid memory leak.
      if (queryId != null) {
        DrillClient drillClient = ((DrillConnection) connection).getClient();
        drillClient.resumeQuery(QueryIdHelper.getQueryIdFromString(queryId));
      }
    }
  }
}
