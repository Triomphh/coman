#pragma once

#include "../../third_party/crow_all.h"
#include "../services/IUserService.hpp"
#include <memory>

namespace utils {

class ContextUtils 
{
public:
    static void populate_navbar_context(crow::mustache::context& ctx, const crow::request& req, std::shared_ptr<IUserService> user_service) 
    {
        auto& session_ctx = *reinterpret_cast<crow::detail::context<crow::CookieParser, crow::SessionMiddleware<crow::FileStore>>*>(req.middleware_context);
        auto& session = session_ctx.template get<crow::SessionMiddleware<crow::FileStore>>();
        
        auto username = session.get("user", "");
        if (!username.empty()) 
        {
            auto user = user_service->get_user_by_email(username);
            if (user) 
            {
                ctx["profile_picture"] = user->profile_picture;
                ctx["name"] = user->name;
            }
        }
    }
};

} // namespace utils 