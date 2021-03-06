/*
 * ServerValidateUser.hpp
 *
 * Copyright (C) 2009-12 by RStudio, Inc.
 *
 * Unless you have received this program directly from RStudio pursuant
 * to the terms of a commercial license agreement with RStudio, then
 * this program is licensed to you under the terms of version 3 of the
 * GNU Affero General Public License. This program is distributed WITHOUT
 * ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
 * AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
 *
 */

#ifndef SERVER_AUTH_VALIDATE_USER_HPP
#define SERVER_AUTH_VALIDATE_USER_HPP

#include <string>

#include <server/ServerOptions.hpp>

namespace server {
namespace auth {

bool validateUser(
  const std::string& username,
  const std::string& requiredGroup,
  bool groupFailureWarning);

inline bool validateUser(const std::string& username)
{
   return validateUser(username,
                       server::options().authRequiredUserGroup(),
                       true);
}


} // namespace auth
} // namespace server

#endif // SERVER_AUTH_VALIDATE_USER_HPP

