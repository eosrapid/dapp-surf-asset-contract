#include <dappstorage.hpp>

ACTION dappstorage::add(name uploader, checksum256 sha256hash, std::string data) {
  // validate caller permissions as the uploader
  require_auth(uploader);

  // ensure that the sha256hash passed by the user is the real sha256 hash of data
  assert_sha256(data.c_str(), data.length(), sha256hash);

  auto data_hash_index = tbl_resources.get_index<"datahashidx"_n>();
  auto data_hash_iterator = data_hash_index.find(sha256hash);
  eosio::check(data_hash_iterator == data_hash_index.end(), "resource already exists");

  uint64_t new_rid = tbl_resources.available_primary_key();
  tbl_resources.emplace(uploader, [&](auto &row) {
    row.rid = new_rid;
    row.uploader = uploader;
    row.sha256hash = sha256hash;
    row.data = data;
  });
}
