"""
Google Drive Downloader - Handles downloading files from Google Drive
"""
import requests
import os
from pathlib import Path

class GoogleDriveDownloader:
    def __init__(self, config):
        self.config = config
        self.base_url = "https://drive.google.com/uc?export=download"
        
    def download_version(self, version, log_callback):
        """Download version from Google Drive"""
        # Get file ID from config
        versions_data = self.config.get('versions', {})
        version_info = versions_data.get(version)
        
        if not version_info:
            raise Exception(f"No configuration found for version {version}")
            
        file_id = version_info.get('file_id')
        if not file_id:
            raise Exception(f"No Google Drive file ID found for version {version}")
            
        log_callback(f"Connecting to Google Drive...")
        log_callback(f"File ID: {file_id}")
        
        # Download file
        session = requests.Session()
        
        # Initial request
        response = session.get(self.base_url, params={'id': file_id}, stream=True)
        
        # Handle Google Drive virus scan warning for large files
        token = None
        for key, value in response.cookies.items():
            if key.startswith('download_warning'):
                token = value
                break
                
        if token:
            log_callback(f"Confirming large file download...")
            params = {'id': file_id, 'confirm': token}
            response = session.get(self.base_url, params=params, stream=True)
        
        # Check if we got HTML (error page) instead of file
        content_type = response.headers.get('content-type', '')
        if 'text/html' in content_type:
            raise Exception(
                "Failed to download from Google Drive.\n"
                "The file might be:\n"
                "- Not shared publicly\n"
                "- The file ID is incorrect\n"
                "- Google Drive link restrictions"
            )
        
        # Get file size
        total_size = int(response.headers.get('content-length', 0))
        
        # Save file
        download_dir = Path.cwd() / "downloads"
        download_dir.mkdir(exist_ok=True)
        
        # Use version-specific filename
        download_path = download_dir / f"LEKMOD_{version}.zip"
        
        # Download with progress
        log_callback(f"Downloading to {download_path}...")
        
        if total_size > 0:
            log_callback(f"File size: {self._format_size(total_size)}")
        
        downloaded = 0
        chunk_size = 32768  # 32KB chunks
        
        with open(download_path, 'wb') as f:
            for chunk in response.iter_content(chunk_size=chunk_size):
                if chunk:
                    f.write(chunk)
                    downloaded += len(chunk)
                    
                    # Update progress every MB
                    if downloaded % (1024 * 1024) == 0 or downloaded == total_size:
                        if total_size > 0:
                            progress = (downloaded / total_size) * 100
                            log_callback(
                                f"Downloading... {self._format_size(downloaded)} / "
                                f"{self._format_size(total_size)} ({progress:.1f}%)"
                            )
                        else:
                            log_callback(f"Downloading... {self._format_size(downloaded)}")
        
        log_callback(f"✓ Downloaded successfully!")
        return str(download_path)
    
    def _format_size(self, bytes_size):
        """Format bytes to human-readable size"""
        for unit in ['B', 'KB', 'MB', 'GB']:
            if bytes_size < 1024.0:
                return f"{bytes_size:.1f} {unit}"
            bytes_size /= 1024.0
        return f"{bytes_size:.1f} TB"
    
    def get_file_info(self, file_id):
        """Get file information from Google Drive (optional, requires API key)"""
        # This would require Google Drive API setup
        # For now, return placeholder
        return {
            'name': 'Lekmod.zip',
            'size': 'Unknown',
            'modified': 'Unknown'
        }

