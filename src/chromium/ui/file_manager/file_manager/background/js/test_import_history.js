// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Namespace
var importer = importer || {};

/**
 * importer.ImportHistory and importer.HistoryLoader test double.
 * ONE STOP SHOPPING!
 *
 * @constructor
 * @struct
 * @implements {importer.HistoryLoader}
 * @implements {importer.ImportHistory}
 */
importer.TestImportHistory = function() {
  /** @type {!Object.<string, !Object.<!importer.Destination, string>>} */
  this.copiedPaths = {};

  /** @type {!Object.<string, Array.<string>>} */
  this.importedPaths = {};

};

/** @override */
importer.TestImportHistory.prototype.getHistory =
    function() {
  return Promise.resolve(this);
};

/**
 * @param {!FileEntry} entry
 * @param {!importer.Destination} destination
 */
importer.TestImportHistory.prototype.assertCopied =
    function(entry, destination) {
  assertTrue(this.wasCopied_(entry, destination));
};

/**
 * Fully synchronous version of wasCopied.
 * @param {!FileEntry} entry
 * @param {!importer.Destination} destination
 * @return {boolean}
 */
importer.TestImportHistory.prototype.wasCopied_ =
    function(entry, destination) {
  var path = entry.fullPath;
  return path in this.copiedPaths &&
      this.copiedPaths[path].indexOf(destination) > -1;
};

/** @override */
importer.TestImportHistory.prototype.wasCopied =
    function(entry, destination) {
  var path = entry.fullPath;
  return Promise.resolve(this.wasCopied_(entry, destination));
};

/** @override */
importer.TestImportHistory.prototype.markCopied =
    function(entry, destination) {
  var path = entry.fullPath;
  if (path in this.copiedPaths) {
    this.copiedPaths[path].push(destination);
  } else {
    this.copiedPaths[path] = [destination];
  }
  return Promise.resolve();
};

/**
 * @param {!FileEntry} entry
 * @param {!importer.Destination} destination
 */
importer.TestImportHistory.prototype.assertImported =
    function(entry, destination) {
  assertTrue(this.wasImported_(entry, destination));
};

/**
 * Fully synchronous version of wasImported.
 * @param {!FileEntry} entry
 * @param {!importer.Destination} destination
 * @return {boolean}
 */
importer.TestImportHistory.prototype.wasImported_ =
    function(entry, destination) {
  var path = entry.fullPath;
  return path in this.importedPaths &&
      this.importedPaths[path].indexOf(destination) > -1;
};

/** @override */
importer.TestImportHistory.prototype.wasImported =
    function(entry, destination) {
  var path = entry.fullPath;
  return Promise.resolve(this.wasImported_(entry, destination));
};

/** @override */
importer.TestImportHistory.prototype.markImported =
    function(entry, destination) {
  var path = entry.fullPath;
  if (path in this.importedPaths) {
    this.importedPaths[path].push(destination);
  } else {
    this.importedPaths[path] = [destination];
  }
  return Promise.resolve();
};

/** @override */
importer.TestImportHistory.prototype.addObserver = function() {};

/** @override */
importer.TestImportHistory.prototype.removeObserver = function() {};
